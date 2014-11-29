//===-- AVRMCCodeEmitter.cpp - Convert Mips Code to Machine Code ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AVRMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//
//

#include "AVRMCCodeEmitter.h"
#include "MCTargetDesc/AVRFixupKinds.h"
#include "MCTargetDesc/AVRMCExpr.h"
#include "MCTargetDesc/AVRMCTargetDesc.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "mccodeemitter"

#define GET_INSTRMAP_INFO
#include "AVRGenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

MCCodeEmitter *llvm::createAVRMCCodeEmitter(const MCInstrInfo &MCII,
                                         const MCRegisterInfo &MRI,
                                         const MCSubtargetInfo &STI,
                                         MCContext &Ctx) {
  return new AVRMCCodeEmitter(MCII, Ctx, false);
}

void AVRMCCodeEmitter::EmitByte(unsigned char C, raw_ostream &OS) const {
  OS << (char)C;
}

void AVRMCCodeEmitter::EmitInstruction(uint64_t Val, unsigned Size,
                                        const MCSubtargetInfo &STI,
                                        raw_ostream &OS) const {
  for (unsigned i = 0; i < Size; ++i) {
    unsigned Shift = !IsLittleEndian ? i * 8 : (Size - 1 - i) * 8;
    EmitByte((Val >> Shift) & 0xff, OS);
  }
}

/// EncodeInstruction - Emit the instruction.
/// Size the instruction with Desc.getSize().
void AVRMCCodeEmitter::
EncodeInstruction(const MCInst &MI, raw_ostream &OS,
                  SmallVectorImpl<MCFixup> &Fixups,
                  const MCSubtargetInfo &STI) const
{
  uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);
  
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  
  // Get byte count of instruction
  unsigned Size = Desc.getSize();
  if (!Size)
    llvm_unreachable("Desc.getSize() returns 0");

  EmitInstruction(Binary, Size, STI, OS);
}

/// getMemriEncoding - Return binary encoding of a pointer register plus displacement operand.
/// If the offset operand requires relocation, record the relocation.
unsigned
AVRMCCodeEmitter::getMemriEncoding(const MCInst &MI, unsigned OpNo,
                                   SmallVectorImpl<MCFixup> &Fixups,
                                   const MCSubtargetInfo &STI) const {
                                   
  // the first operand should be the pointer register.
  assert(MI.getOperand(OpNo).isReg());
  
  // the second operand should be the displacement as an immediate value.
  assert(MI.getOperand(OpNo+1).isImm());
  
  unsigned encoding = 0;
  
  uint8_t reg_bit = 0; // register bit.
  uint8_t disp_bits = MI.getOperand(OpNo+1).getImm(); // displacement bits (6 bits).
  
  switch(MI.getOperand(OpNo).getReg())
  {
      case AVR::R29R28: // Y pointer register
      {
          reg_bit = 1;
          
          break;
      }
      case AVR::R31R30: // Z pointer register
      {
          reg_bit = 0;
          
          break;
      }
      case AVR::R27R26: // X pointer register (not supported).
      {
          llvm_unreachable("cannot encode the X pointer register for this encoding");
          
          break;
      }
      default: // Some other register we don't support.
      {
          llvm_unreachable("register not supported for this operand encoding");
          
          break;
      }
  }
  
  encoding = (disp_bits<<1) | reg_bit;

  return encoding;
}

unsigned
AVRMCCodeEmitter::getBreakTargetEncoding(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
                                   
  const MCOperand MO = MI.getOperand(OpNo);
  
  if (MO.isExpr())
  {
    const MCOperand &MO = MI.getOperand(OpNo);

    const MCExpr *Expr = MO.getExpr();
    MCFixupKind Kind = MCFixupKind(AVR::fixup_brcond);
    Fixups.push_back(MCFixup::Create(0, Expr, Kind, MI.getLoc()));
    
    // All of the information is in the fixup.
    return 0;
  } 
  else
    return (MO.getImm() >> 1);
}

unsigned AVRMCCodeEmitter::
getExprOpValue(const MCExpr *Expr,SmallVectorImpl<MCFixup> &Fixups,
               const MCSubtargetInfo &STI) const {
               
  MCExpr::ExprKind Kind = Expr->getKind();

  if (Kind == MCExpr::Binary) {
    Expr = static_cast<const MCBinaryExpr*>(Expr)->getLHS();
    Kind = Expr->getKind();
  }

  assert (Kind == MCExpr::SymbolRef);

  // All of the information is in the fixup.
  return 0;
}

unsigned AVRMCCodeEmitter::
getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                  SmallVectorImpl<MCFixup> &Fixups,
                  const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    unsigned RegNo = Ctx.getRegisterInfo()->getEncodingValue(Reg);
    return RegNo;
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  } else if (MO.isFPImm()) {
    return static_cast<unsigned>(APFloat(MO.getFPImm())
        .bitcastToAPInt().getHiBits(32).getLimitedValue());
  }
  
  // MO must be an Expr.
  assert(MO.isExpr());
  
  return getExprOpValue(MO.getExpr(),Fixups, STI);
}

#include "AVRGenMCCodeEmitter.inc"