ORG 10		/ program entry point

/ ビットシフトしながら筆算の要領で計算
DIV,  LDA BIT
      ADD DCM
      STA BIT
      SNA
      BUN L0
      BUN FIN

/ XからYに1ビットずつコピーしていく
L0,   CLE
      LDA X
      CIL
      STA X
      LDA Y
      CIL
      SZE
      INC
      STA Y
      CLE
      LDA QUO
      CIL
      STA QUO
/ YとZの大小関係を比較
      CLE
      LDA Z
      BSA COMP
      ADD Y
      CME
      SZE
      BUN DIV
      SPA
      BUN DIV
      STA Y
      LDA QUO
      INC
      STA QUO
      BUN DIV

/ 最後の処理
FIN,  LDA Y
      STA MOD
      HLT

/ ACの2の補数を求める
COMP, HEX 0
      CMA
      INC
      BUN COMP I

/ data
DCM,  DEC -1    / デクリメント用
BIT,  DEC 16    / ビットシフト用

X,	DEC 4  / X:init(The number to be divided)
Y,  DEC 0      / Xを左シフトした際のビット格納
Z,	DEC 2  / Z:init(number divided by)

QUO,  DEC 0    / result(quoent X / Y)
MOD,  DEC 0    / result(mod X / Y)

END
