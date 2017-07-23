#include "asmparser.h"

class EX3_ASMParser : public ASMParser
{
public:
	EX3_ASMParser(CPU * cpu0, const char * fname) : ASMParser(cpu0, fname)
	{}
	int ParseInsn(int passNum, const char * p, InsnSet::Insn * insn, int & addr);
};