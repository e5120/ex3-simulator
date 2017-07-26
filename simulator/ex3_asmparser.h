#pragma once
#include "ex3_insnset.h"
#include "asmparser.h"

class EX3_ASMParser : public ASMParser
{
public:
	EX3_ASMParser(const std::shared_ptr<CPU>& cpu0, const std::string& fname);
	int ParseInsn(int passNum, std::string& p, const InsnSet::Insn& insn, int addr);
};