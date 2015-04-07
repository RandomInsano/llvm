//===-- AVRAsmBackend.h - AVR Asm Backend  -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the AVRAsmBackend class.
//
//===----------------------------------------------------------------------===//
//

#ifndef LLVM_LIB_TARGET_AVR_MCTARGETDESC_AVRASMBACKEND_H
#define LLVM_LIB_TARGET_AVR_MCTARGETDESC_AVRASMBACKEND_H

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/ADT/Triple.h"

namespace llvm {

class MCAssembler;
struct MCFixupKindInfo;
class Target;
class MCObjectWriter;

class AVRAsmBackend : public MCAsmBackend {

  Triple::OSType OSType;
  bool IsLittle; // Big or little endian

public:
  AVRAsmBackend(const Target &T, Triple::OSType _OSType, bool _IsLittle)
      : MCAsmBackend(), OSType(_OSType), IsLittle(_IsLittle) {}

  MCObjectWriter *createObjectWriter(raw_ostream &OS) const override;

  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value, bool IsPCRel) const override;

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  unsigned getNumFixupKinds() const override {
    return AVR::NumTargetFixupKinds;
  }

  /// @name Target Relaxation Interfaces
  /// @{

  /// MayNeedRelaxation - Check whether the given instruction may need
  /// relaxation.
  ///
  /// \param Inst - The instruction to test.
  bool mayNeedRelaxation(const MCInst &Inst) const override {
    return false;
  }

  /// fixupNeedsRelaxation - Target specific predicate for whether a given
  /// fixup requires the associated instruction to be relaxed.
   bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                             const MCRelaxableFragment *DF,
                             const MCAsmLayout &Layout) const override {
    // FIXME.
    llvm_unreachable("RelaxInstruction() unimplemented");
    return false;
  }

  /// RelaxInstruction - Relax the instruction in the given fragment
  /// to the next wider instruction.
  ///
  /// \param Inst - The instruction to relax, which may be the same
  /// as the output.
  /// \param [out] Res On return, the relaxed instruction.
  void relaxInstruction(const MCInst &Inst, MCInst &Res) const override {}

  /// @}

  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override;
  
  void processFixupValue(const MCAssembler &Asm, const MCAsmLayout &Layout,
                         const MCFixup &Fixup, const MCFragment *DF,
                         const MCValue &Target, uint64_t &Value,
                         bool &IsResolved) override;

}; // class AVRAsmBackend

} // namespace

#endif
