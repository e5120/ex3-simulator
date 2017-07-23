	ORG 0		// interrupt entry point
ST0,HEX 0		// interrupt return address
	BUN I_HND	// goto I_HND (interrupt handler)

	ORG 10		// program entry point
INI, // initialize data
	CLA			// AC <- 0
	STA STT		// M[STT] <- 0
	STA SFG		// M[SFG] <- 0
	STA PFG		// M[PFG] <- 0
// IMSK[3:0] : { S_IN, S_OUT, P_IN, P_OUT }
	LDA VHA		// AC <- M[VHA] (1010)
	STA MSK		// M[MSK] <- (1010)
	IMK			// IMSK   <- (1010) (S_IN, P_IN enabled)
	//ION			// enable interrupt
	CLE
	CME
	IST
	// set timer 10ms
	// LDA VHA
	// TMR

L0, LDA STT		// AC <- M[STT]
	SNA			// (M[STT] < 0) ? skip next
	BUN L0
	HLT
////////////////////// subroutine (update IMSK) ////////////////
UMK,HEX 0
// arg0 (AC) : unchanged bit mask
// arg1 : added mask bit
	AND MSK		// AC <- arg0 & M[MSK]
	ADD UMK I	// AC <- AC + M[M[UMK]]
	STA MSK		// M[MSK] <- AC
	IMK			// IMSK <- AC[3:0]
	ISZ UMK		// ++M[UMK]
	BUN UMK I	// return from UMK
////////////////////// subroutine (check end-character) ////////////////
CEC,HEX 0
// arg0 (AC) : output character
// end-character = 0x4 (ctrl-D)
	ADD VM4		// AC <- AC - 4
	SZA			// (AC == 0) ? skip next
	BUN CEC I	// return from CEC
// output character matches (ctrl-D)
	LDA VM1		// AC <- -1
	STA STT		// M[STT] <- -1
	CLA			// AC <- 0
	IMK			// IMASK <- 0 (all interrupts disabled)
	BUN CEC I	// return from CEC
////////////////////// interrupt handler //////////////////
// 1. store AC & E to memory
I_HND, STA BA	// M[BA] <- AC	(store AC)
	CIL			// AC[0] <- E	(AC[15:1] is not important here...)
	STA BE		// M[BE] <- AC	(store E)
// 2. check SFG and S_IN
//_B_,
I1,
SIN,
	LDA SFG		// AC <- M[SFG]
	SZA			// (M[SFG] == 0) ? skip next
	BUN I2		// goto POU
    SIO			// IOT <- 1 (serial-IO selected)
	SKI			// (UART_RX ready) ? skip next
	BUN I2		// goto PIN
// S_IN is ready --> update IMSK (disable S_IN, enable P_OUT)
	LDA VH6		// AC <- (0110) (S_OUT, P_IN : unchanged)
	BSA UMK		// call UMK (update IMSK)
	HEX 1		// (0001) (P_OUT : enabled)
// read S_IN data
	INP			// AC(7:0) <- INPR
	STA SDT		// M[SDT] <- AC
	ISZ SFG		// ++M[SFG]

CHECK0,
	LDA SDT
	ADD CH_L
	SZA 
	BUN CHECK1	// INPR != L
	LDA BAR1_X	// INPR == L
	ADD VH1
	STA BAR1_X
	ADD WIDTH_
	SZA
	BUN	CHECK1	// BAR1_X + 6 < WIDTH
	LDA WIDTH__	// BAR1_X + 6 = WIDTH
	STA BAR1_X

CHECK1,
	LDA SDT
	ADD CH_F	
	SZA
	BUN CHECK2 //INPR != F
	LDA BAR1_X //INPR == F
	ADD VM1
	STA BAR1_X
	SNA
	BUN CHECK2	// BAR1_X >= 0
	LDA VH0		// BAR1_X < 0
	STA BAR1_X

CHECK2,

// 3. check P_OUT
I2,
PIN,
    PIO			// IOT <- 0 (parallel-IO selected)
	SKI			// (GP_IN ready) ? skip next
	BUN IRT		// goto IRT
	// P_IN is ready --> update IMSK (disable P_IN, enable S_OUT)
	LDA VH9		// AC <- (1001) (S_IN, P_OUT : unchanged)
	BSA UMK		// call UMK (update IMSK)
	HEX 4		// (0100) (S_OUT : enabled)
	// read P_IN data
	INP			// AC(7:0) <- INPR
	STA P I

IN_CHECK1,
	LDA P I
	CMA
	INC
	ADD BX 
	SZA
	BUN	IN_CHECK2	// INP != 'X'
	LDA BALL_X_P 	// INP == 'X'
	STA P
	BUN IN_CHECK5

IN_CHECK2,
	LDA P I
	CMA
	INC
	ADD BY 
	SZA
	BUN	IN_CHECK3	// INP != 'Y'
	LDA BALL_Y_P 	// INP == 'Y'
	STA P
	BUN IN_CHECK5

IN_CHECK3,
	LDA P I
	CMA
	INC
	ADD BAR 
	SZA
	BUN	IN_CHECK4	// INP != 'B'
	LDA BAR2_X_P 	// INP == 'B'
	STA P
	BUN IN_CHECK5

IN_CHECK4,
	LDA INBUF_P
	STA P

IN_CHECK5,

	LDA PDT
	ADD VM10
	SZA				// AC == '\n' ? 
	BUN I3
	ISZ PFG		// ++M[PFG]

I3,
	// SKT
	// BUN I4
	// LDA VHA
	// TMR
POU, // M[SFG] != 0
	BSA BALL_UPDATE
    PIO			// IOT <- 0 (parallel-IO selected)
	SKO			// (P_OUT ready) ? skip next
	BUN I4		// goto PIN
// P_OUT is ready --> update IMSK (disable P_OUT, enable S_IN)
	LDA VH6		// AC <- (0110) (S_OUT, P_IN : unchanged)
	BSA UMK		// call UMK (update IMSK)
	HEX 8		// (1000) (S_IN : enabled)
// output to P_OUT
SEND1,			//BALL_X
	PIO
	SKO
	BUN SEND1
	LDA BX
	OUT
SEND2,
	PIO
	SKO
	BUN SEND2
	LDA BALL_X
	// ADD VH48
	OUT
SEND3,			//BALL_Y
	PIO
	SKO
	BUN SEND3
	LDA BY
	OUT
SEND4,
	PIO
	SKO
	BUN SEND4
	LDA BALL_Y
	// ADD VH48
	OUT
SEND5,			//BAR1_X
	PIO
	SKO
	BUN SEND5
	LDA BAR
	OUT
SEND6,
	PIO
	SKO
	BUN SEND6
	LDA BAR1_X
	ADD WIDTH
	CMA
	INC
	OUT
	
	BSA CEC		// call CEC (check end-character)
	CLA			// AC <- 0
	STA SFG		// M[SFG] <- 0
// 4. check PFG and P_IN

	

// 5. check S_OUT 

SOU, // M[PFG] != 0
    SIO			// IOT <- 1 (serial-IO selected)
	// S_OUT is ready --> update IMSK (disable S_OUT, enable P_IN)
	LDA VH9		// AC <- (1001) (S_IN, P_OUT : unchanged)
	BSA UMK		// call UMK (update IMSK)
	HEX 2		// (0010) (P_IN : enabled)

SOU1, 				// BALL_X
	SKO
	BUN SOU1
	LDA BALL_X
	ADD VH2
	OUT
SOU2, 				// BALL_Y
	SKO
	BUN SOU2
	LDA BALL_Y
	ADD VH2
	OUT
SOU3, 				// BAR2_X
	SKO
	BUN SOU3
	LDA BAR2_X
	ADD VH2
	OUT
SOU4, 				// BAR1_X
	SKO
	BUN SOU4
	LDA BAR1_X
	ADD VH2
	OUT

	CLA			// AC <- 0
	STA PFG		// M[SFG] <- 0
// 6. restore AC & E from memory
I4,
IRT,
	// BSA BALL_UPDATE
	LDA BE		// AC <- M[BE]
	CIR			// E <- AC[0]	(restore E)
	LDA BA		// AC <- M[BA]	(restore AC)
	//ION			// IEN <- 1		(enable interrupt)
	CLE
	CME
	IST
	BUN ST0 I	// indirect return (return address stored in ST0)


BALL_UPDATE,
	HEX 0
BU0,
	LDA BALL_X
	ADD BALL_VX
	STA BALL_X
	LDA BALL_Y
	ADD BALL_VY
	STA BALL_Y
	SZA
	BUN BU1			// BALL_Y != 0
	LDA BAR1_X
	BSA COL_CHECK 	// BALL_Y == 0
	LDA BALL_VY
	SNA
	BUN BU1			// BALL_VY >= 0
	LDA BALL_VY		// BALL_VY < 0
	CMA
	INC
	STA BALL_VY

BU1,
	LDA BALL_Y
	ADD HEIGHT
	SZA
	BUN	BU2			// BALL_Y != 30 
	LDA BAR2_X
	BSA COL_CHECK 	// BALL_Y == 30
	LDA BALL_VY
	SPA
	BUN BU1			// BALL_VY < 0
	LDA BALL_VY		// BALL_VY >= 0
	CMA
	INC
	STA BALL_VY

BU2,
	LDA BALL_X
	SZA
	BUN BU3		// BALL_X != 0
	LDA BALL_VX	// BALL_X == 0
	CMA			// BALL_VX *= -1
	INC
	STA BALL_VX
BU3,
	LDA BALL_X
	ADD WIDTH
	SZA
	BUN	BU4		// BALL_X != 44 
	LDA BALL_VX	// BALL_X == 44
	CMA 		// BALL_VX *= -1
	INC
	STA BALL_VX
BU4,	
	BUN BALL_UPDATE I


COL_CHECK, 
	HEX 0
	CMA
	INC

CC0,
	ADD BALL_X
	CMA
	INC
	STA B_BUF
	// SPA
	// //BUN COL_CHECK I
	// BUN CC6 	// BALL_X < BAR1_X 
CC1, 			// BALL_X >= BAR1_X
	LDA B_BUF
	SZA 				
	BUN CC2 	// != 1
	LDA VM2		// == 1
	STA BALL_VX 	// VX = -2
	LDA VH1
	STA BALL_VY 	// VY = 1
	BUN CC6
CC2,
	ISZ B_BUF
	// LDA B_BUF
	// SZA 				
	BUN CC3 	// != 2
	LDA VM1		// == 2
	STA BALL_VX 	// VX = -1
	LDA VH1
	STA BALL_VY 	// VY = 1
	BUN CC6
CC3,
	ISZ B_BUF
	// LDA B_BUF
	// SZA 				
	BUN CC4 	// != 3
	LDA VH0		// == 3
	STA BALL_VX 	// VX = 0
	LDA VH2
	STA BALL_VY 	// VY = 2
	BUN CC6
CC4,
	ISZ B_BUF
	// LDA B_BUF
	// SZA 				
	BUN CC5 	// != 4
	LDA VH1		// == 4
	STA BALL_VX 	// VX = 1
	LDA VH1
	STA BALL_VY 	// VY = 1
	BUN CC6
CC5,
	ISZ B_BUF
	// LDA B_BUF
	// SZA 				
	//BUN COL_CHECK I 	// != 5
	BUN CC6 	// != 5
	LDA VH2		// == 5
	STA BALL_VX		// VX = 2
	LDA VH1
	STA BALL_VY 	// VY = 1
	BUN CC6
CC6,
	BUN COL_CHECK I


// data (no initialization)
BA, DEC 000		// backup storage for AC during interrupt handling
BE, DEC 000		// backup storage for  E during interrupt handling
SDT,DEC 0		// S_IN data
PDT,DEC 0		// P_IN data
// data (need initialization code)
STT,DEC 0       // state
SFG,DEC 0       // S_IN flag
PFG,DEC 0       // P_IN flag
MSK,DEC 0		// IMSK data
// data (read-only)

VH0,DEC 0
VH1,DEC 1
VH2,DEC 2
VH6,HEX 6       // VH6 = 0x6 (0110)
VH9,HEX 9       // VH9 = 0x9 (1001)
VHA,HEX A       // VHA = 0xA (1010)

VH100,DEC 100
VM1,DEC -1      // VM1 = -1
VM2,DEC -2
VM4,DEC -4		// VM4 = -4

VH48,DEC 48

VD4,  DEC 4
VD0,		DEC 0
VM5, DEC -5
VM10, DEC -10
BX,	CHR X
BY,	CHR Y
BAR,CHR B


CH_L,	DEC -108
CH_F,	DEC -102

COUNT,	DEC 0

M4P,	SYM M4
M4,		DEC 32
		CHR >
		DEC 32
		DEC 0


WIDTH,	DEC -44
HEIGHT,	DEC -30
WIDTH_, DEC -40
WIDTH__, DEC 39

P,	SYM INBUF_P
BALL_X_P,	SYM BALL_X
BALL_Y_P,	SYM BALL_Y
BAR1_X_P,	SYM BAR1_X
BAR2_X_P,	SYM BAR2_X
INBUF_P,	SYM INBUF

B_BUF, DEC 0
BALL_VX,DEC 1
BALL_VY,DEC 1
BALL_X, DEC 15
BALL_Y, DEC 15
BAR1_X, DEC 15
BAR2_X,	DEC 15
INBUF,	HEX 0


BUFC,	SYM BUF
BUFP,	SYM BUF
SBUFP,	SYM BUF
RBUFP,	SYM BUF
BUF,	DEC 0
END

