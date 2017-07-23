ORG 0		/ interrupt entry point
ST0,HEX 0		/ interrupt return address
BUN I_HND	/ goto I_HND (interrupt handler)

ORG 10		/ program entry point
INI, / initialize data
  CLA			/ AC <- 0
  STA STT		/ M[STT] <- 0
  STA SFG		/ M[SFG] <- 0
  / IMSK[3:0] : { S_IN, S_OUT, P_IN, P_OUT }
  LDA VH4		/ AC <- M[VH8] (1000)
  IMK			/ IMSK <- (1000) (S_IN enabled)
  SIO			/ IOT <- 1 (serial-IO selected)
  CLE
  CME
  IST

L0, LDA STT		/ AC <- M[STT]
    SNA			/ (M[STT] < 0) ? skip next
    BUN L0
    HLT

/////////// interrupt handler /////////
/ 1. store AC & E to memory
I_HND, STA BA	/ M[BA] <- AC	(store AC)
      CIL			/ AC[0] <- E	(AC[15:1] is not important here...)
      STA BE		/ M[BE] <- AC	(store E)

/ 3. check GP_OUT
SOU, / M[SFG] != 0
  SKO			/ (S_OUT ready) ? skip next
  BUN IRT		/ goto IRT
  / output to S_OUT
  LDA ST I 		/ AC <- M[SDT]
  OUT
  ISZ ST
  ISZ COUNT
  BUN SOU
  HLT
/ 4. restore AC & E from memory
IRT,LDA BE		/ AC <- M[BE]
  CIR			/ E <- AC[0]	(restore E)
  LDA BA		/ AC <- M[BA]	(restore AC)
  CLE
  CME
  IST
  BUN ST0 I	/ indirect return (return address stored in ST0)

/ data (no initialization)
BA, DEC 000		/ backup storage for AC during interrupt handling
BE, DEC 000		/ backup storage for  E during interrupt handling
SDT,DEC 0		/ S_IN data
/ data (need initialization code)
STT,DEC 0       / state
SFG,DEC 0       / S_IN flag
/ data (read-only)
VH4,HEX 4       / VH4 = 0x4 (0100)
VH8,HEX 8       / VHA = 0x8 (1000)
VM1,DEC -1      / VM1 = -1
VM4,DEC -4		/ VM4 = -4

COUNT, DEC -14
STR, HEX 1b
   CHR [
   CHR 3
   CHR 3
   CHR m
   CHR T
   CHR E
   CHR S
   CHR T
   HEX 1b
   CHR [
   CHR 3
   CHR 9
   CHR m
ST, SYM STR
END
