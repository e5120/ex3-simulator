#pragma once
#include "asmparser.h"

class EX3_ASMParser : public ASMParser
{
public:
	EX3_ASMParser(const std::shared_ptr<CPU>& cpu0, const std::string fname) : ASMParser(cpu0, fname)
	{}
	int ParseInsn(int passNum, const char * p, InsnSet::Insn * insn, int& addr);
};