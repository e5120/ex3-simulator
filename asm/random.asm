ORG 0						/ interrupt entry point
ST0,	HEX 0			/ interrupt return address
			BUN I_HND	/ goto I_HND (interrupt handler)

ORG 10					/ program entry point
INI, 						/ initialize data
		CLA					/ AC <- 0
		STA STT			/ M[STT] <- 0
		LDA VH4			/ AC <- M[VH4] (0100)
		IMK					/ IMSK <- (0100) (S_OUT enabled)
		SIO					/ IOT <- 1 (serial-IO selected)
		CLE					/ E <- 0
		CME					/ E <- ~E
		IST					/ IEN <- 1

L0, LDA STT			/ AC <- M[STT]
		SNA					/ (M[STT] < 0) ? skip next
		BUN L0
		HLT

/////////// interrupt handler /////////
/ 1. store AC & E to memory
I_HND,	STA BA		/ M[BA] <- AC	(store AC)
				CIL				/ AC[0] <- E	(AC[15:1] is not important here...)
				STA BE		/ M[BE] <- AC	(store E)

/ 3. check GP_OUT
SOU, 						/ M[SFG] != 0
			SKO				/ (S_OUT ready) ? skip next
			BUN IRT		/ goto IRT
/ output to S_OUT
      LDA MAX
      RND
			ADD AD
			OUT				/ OUTR <- AC
      LDA CL
      OUT
			ISZ CNT
			BUN IRT
			HLT

/ 4. restore AC & E from memory
IRT,	LDA BE			/ AC <- M[BE]
			CIR					/ E <- AC[0]	(restore E)
			LDA BA			/ AC <- M[BA]	(restore AC)
			CLE					/ E <- 0
			CME					/ E <- ~E
			IST					/ IEN <- 1
			BUN ST0 I		/ indirect return (return address stored in ST0)

/ data (no initialization)
BA,		DEC 000		/ backup storage for AC during interrupt handling
BE, 	DEC 000		/ backup storage for  E during interrupt handling
/ data (need initialization code)
STT,	DEC 0     / state
/ data (read-only)
VH4,	HEX 4     / VH4 = 0x4 (0100)
VM1,	DEC -1    / VM1 = -1
VM4,	DEC -4		/ VM4 = -4
AD, DEC 48			/ ASCII '0'
CL, HEX A       / ASCII '\n'
CNT, DEC -10		/ count
MAX, DEC 10   / max value of random
END
