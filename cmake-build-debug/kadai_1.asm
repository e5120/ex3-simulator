ORG 10		/ program entry point

L0, CLE         / E <- 0
    LDA YL      / AC <- M[YL]
    SZA         / (AC == 0) ? skip next
    BUN LY      / goto LY
    HLT

LY, CIR         / {AC[15:0], E} <- {E, AC[15:0]}
    STA YL      / M[YL] <- AC
    SZE			    / (E == 0) ? skip next
    BUN LP		  / goto LP

LX, LDA XL		  / AC <- M[XL]
    CIL			    / {E, AC[15:0]} <- {AC[15:0], E}
    STA XL		  / M[XL] <- AC
    LDA XH      / AC <- M[XH]
    CIL         / {E, AC[15:0]} <- {AC[15:0], E}
    STA XH      / M[XH] <- AC
    BUN L0		  / goto LO

LP, LDA XL		  / AC <- M[XL]
    ADD PL		  / AC <- AC + M[PL]
    STA PL		  / M[PL} <- AC
    CLA         / AC <- 0
    CIL         / {E, AC[15:0]} <- {AC[15:0], E}
    ADD PH      / AC <- AC + M[PH]
    ADD XH      / AC <- AC + M[XH]
    STA PH      / M[PH] <- AC
    CLE			    / E <- 0
    BUN LX		  / goto LX


/ data
XL, DEC 10		/ X:init(low digit)
XH, DEC 0       / X:init(high digit)
YL, DEC 10		/ Y:init(low digit)
YH, DEC 0       / Y:init(high digit)
PL, DEC 0		    / result(low digit)
PH, DEC 0       / result(high digit)
END
