# Supported Instructions

## Key
* `x` means that the instruction is supported and can be compiled to machine code
* `pseudo` means that the instruction is supported, but only in assembly output (no machine code support)
* `not implemented` means that no support exists whatsoever


### Arithmetic and Logic Instructions
```
ADD    x
ADC    x
ADIW   x
SUB    x
SUBI   x
SBC    x
SBCI   x
SBIW   x
AND    x
ANDI   x
OR     x
ORI    x
EOR    x
COM    x
NEG    x
SBR    x (untested)
CBR    not implemented
  * (I'm fairly sure we need to add a pseudo instruction and lower into compliment and ANDI)
INC    x
DEC    x
DEC    x
TST    x (untested)
CLR    x (untested)
SER    x (untested)
MUL    x
MULS   not implemented
MULSU  not implemented
FMUL   not implemented
FMULS  not implemented
FMULSU not implemented
DES    not implemented
SBIC   not implemented
SBIS   not implemented
```

### Branch Instructions
```
RJMP    x
IJMP    x (untested)
EIJMP   not implemented
JMP     x
RCALL   x (untested)
ICALL   x
EICALL  not implemented
CALL    x
RET     x
RETI    x
CPSE    not implemented
CP      x
CPC     x
CPI     x (untested)
SBRC    not implemented
SBRS    not implemented
SBIC    not implemented
SBIS    not implemented
BRBS    not implemented
BRBC    not implemented
BREQ    x
BRNE    x
BRCS    x (untested)
BRCC    x (untested)
BRSH    x
BRLO    x
BRMI    x
BRPL    x
BRGE    x
BRLT    x
BRHS    x (untested)
BRHC    x (untested)
BRTS    x (untested)
BRTC    x (untested)
BRVS    x (untested)
BRVC    x (untested)
BRIE    x (untested)
BRID    x (untested)
```

### Data Transfer Instructions
```
MOV            x
MOVW           x
LDI            x
LDS            x
LD Rr, P       x
LD Rr, P+      x (untested)
LD Rr, -P      x (untested)
LDD            x
STS            x (untested)
ST Ptr, Rd     x
ST P+, Rr      x (untested)
ST -P, Rr      x (untested)
STD            x
LPM            not implemented (this variant implies Rd = r0)
LPM Rd, Z      x (untested)
LPM Rd, Z+     x (untested)
ELPM           not implemented
SPM            not implemented
SPM (postinc variant) not implemented
IN             x
OUT            x
PUSH           x
POP            x
XCH
LAS
LAC
LAT
```

### Bit and Bit-test instructions
```
LSL    x
LSR    x
ROL    x
ROR    x
ASR    x
SWAP   x
BSET   not implemented
BCLR   not implemented
SBI    x (untested)
CBI    x (untested)
BST    not implemented
BLD    not implemented
SEC    x (untested)
CLC    x (untested)
SEN    x (untested)
CLN    x (untested)
SEZ    x (untested)
CLZ    x (untested)
SEI    x (untested)
CLI    x (untested)
SES    x (untested)
CLS    x (untested)
SEV    x (untested)
CLV    x (untested)
SET    x (untested)
CLT    x (untested)
SEH    x (untested)
CLH    x (untested)
```

### MCU Control Instructions
```
BREAK    x (untested)
NOP      x (untested)
SLEEP    x (untested)
WDR      x (untested)
```
