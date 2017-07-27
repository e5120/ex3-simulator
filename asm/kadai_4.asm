ORG 10		/ program entry point
/ 作業メモリに入力値をコピー
INIT, CLE
      LDA MAX
      STA TMP
      BUN ROOT

/ 入力値の平方根を求める（素数判定に必要）
ROOT, BSA SUB
      SZE
      BUN CHECK
      SPA
      BUN PRIME
      BUN CHECK

/ 入力値からCOUNTの値だけ引いていく
SUB,  HEX 0
      LDA COUNT
      BSA COMP
      ADD TMP
      STA TMP
      BUN SUB I

/ 平方根のアルゴリズムに必要なサブルーチン呼び出し
CHECK,  BSA SQUAREADD
        BSA COUNTADD
        BUN ROOT

/ 平方根を格納するメモリの値をインクリメント
SQUAREADD,  HEX 0
            LDA SQUARE
            INC
            STA SQUARE
            BUN SQUAREADD I

/ COUNTメモリを2だけインクリメント
COUNTADD, HEX 0
          LDA COUNT
          INC
          INC
          STA COUNT
          BUN COUNTADD I

/ 2の補数を求める
COMP, HEX 0
      CMA
      INC
      BUN COMP I

/ 素数を求めるアルゴリズムの開始
PRIME,  CLA
        INC
        STA DIV
        BUN L0

/ 対象となる値を試し割りし、割り切れなければ素数として終了
L0, LDA DIV
    BSA COMP
    ADD SQUARE
    SPA
    BUN FIN
    LDA MAX
    STA TMP
    CLA
    STA CP
    LDA DIV
    INC
    STA DIV
    LDA BIT
    STA TBIT
    BUN L1

/ 16bit分L2ラベルにとぶ
L1, LDA TBIT
    ADD DCM
    STA TBIT
    SNA
    BUN L2
    BUN L3

/ 除算アルゴリズム
L2, CLE
    LDA TMP
    CIL
    STA TMP
    LDA CP
    CIL
    SZE
    INC
    STA CP
    CLE
    LDA DIV
    BSA COMP
    ADD CP
    SZE
    STA CP
    SNA
    STA CP
    BUN L1

/　割り切れているか判定
L3, LDA CP
    SZA
    BUN L0
    LDA MAX
    ADD DCM
    STA MAX
    BUN PRIME

/ 終了時処理
FIN, HLT

/ data  find max prime number
COUNT,  DEC 1     / subtraction value
DIV,    DEC 0     / divide value
DCM,    DEC -1    / デクリメント用
BIT,    DEC 16    / 割り算をする時に16bitだけシフトするためのメモリ
TBIT,   DEC 0     / BITは何度も利用するので値をいじるようのメモリ

TMP,    DEC 0     / 入力値(MAX)をコピーしてこのメモリで作業
CP,     DEC 0     / 除算時にTMPをシフトしてきたものを保存

SQUARE, DEC 0     / 入力値の平方根が入る
MAX,    DEC 65535 / init:input value, fin:prime value
END
