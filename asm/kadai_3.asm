ORG 10		/ program entry point
/ ASCII文字を数値に変換
ASCII,  LDA PIN I
        ADD CHECK2
        SPA
        BUN 1TO9
        BUN ATOF
        ISZ PIN

/ BINを４桁左シフトする
SHIFT,  CLE
        LDA BIN
        CIL
        CIL
        CIL
        CIL
        STA BIN
        BUN ASCII

/ 入力値が１〜９か判定
1TO9, LDA PIN I
      ADD CHECK1
      SPA
      BUN ERROR
      BUN AD

/ 入力値がA〜Fか判定
ATOF, LDA PIN I
      ADD CHECK4
      SNA
      BUN ERROR
      LDA PIN I
      ADD CHECK3
      SPA
      BUN ERROR
      ADD CHECK5
      BUN AD

/ 入力値が16進表記として有効な時SHIFTラベルにとぶ
AD, ADD BIN
    STA BIN
    LDA DIGIT
    ADD DCM
    STA DIGIT
    ISZ PIN
    SZA
    BUN SHIFT
    BUN 16TO10

/ 数値に変換された16進文字列を10進文字列に変換
16TO10, CLA
        STA QUO
        LDA BIT
        STA TBIT
        LDA DIV
        SZA
        BUN L0
        BUN FIN

/ 数値(BIN)を1000〜10の値で割って各桁の数値を取得
L0,   LDA TBIT
      ADD DCM
      STA TBIT
      SNA
      BUN DIV1
      BUN CONVERT

/ 割り算部分
DIV1, CLE
      LDA BIN
      CIL
      STA BIN
      LDA CP
      CIL
      SZE
      INC
      STA CP
      CLE
      LDA QUO
      CIL
      STA QUO
      CLE
      LDA DIV
      BSA COMP
      ADD CP
      CME
      SZE
      BUN L0
      SPA
      BUN L0
      STA CP
      LDA QUO
      INC
      STA QUO
      BUN L0

/ 取得した桁に48を足し、ASCII文字列の数字に変換
CONVERT,  LDA CHECK1
          BSA COMP
          ADD QUO
          STA POUT I
          ISZ POUT
          LDA CP
          STA BIN
          CLA
          STA QUO
          STA CP
          BUN DIV2

/ 1000から各桁を取得するごとに10で除算
DIV2, LDA TMP
      BSA COMP
      ADD DIV
      SPA
      BUN RESET
      STA DIV
      LDA QUO
      INC
      STA QUO
      BUN DIV2

RESET, LDA QUO
       STA DIV
       BUN 16TO10

/ 2の補数(負数)を求める
COMP, HEX 0
      CMA
      INC
      BUN COMP I

/ 正常終了時の処理記述
FIN,  LDA CHECK1
      BSA COMP
      ADD BIN
      STA POUT I
      HLT

/ 異常終了(無効な16進表記の入力文字列)時の処理記述
ERROR,  HLT

/ data
DCM,    DEC -1     / デクリメント用
DIGIT,  DEC 4
DIV,    DEC 10000 / 取得値を４桁の数値の文字列に分割するとき使用
TMP,    DEC 10    / DIVを１０ずつ割っていく時使用
QUO,    DEC 0     / 商を格納

/ 配列(IN, OUT)を指すポインタ
PIN,  SYM IN
POUT, SYM OUT

/ ASCIIコードのチェック用
CHECK1, DEC -48
CHECK2, DEC -58
CHECK3, DEC -65
CHECK4, DEC -71
CHECK5, DEC 10

/ 除算時のビットカウント用
BIT,  DEC 16
TBIT, DEC 0

BIN,  DEC 0    / 入力値を２進数に変換した値
CP,   DEC 0    / BINのbitを一つずつ受け取る

/ 入力値（１６進数）
IN, CHR  0
	  CHR  0
	  CHR  0
	  CHR  7

/ 出力値（１０進数）
OUT,  CHR  E
	    CHR  R
	    CHR  R
      CHR  O
      CHR  R
END
