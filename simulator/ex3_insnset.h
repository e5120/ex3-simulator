#pragma once
#include "insnset.h"
#include <stdlib.h>
#include <time.h>

class EX3_InsnSet : public InsnSet
{
public:
	enum InsnType
	{
		NON_INSN, MEM_INSN, REG_INSN,
	};
	enum InsnID
	{
		I_ANCHOR = InsnSet::I_TAIL - 1,
#define INSN_ENUM
#include "ex3_asm_def.h"
#undef INSN_ENUM
		I_COUNT
	};

#define INSN_FUNC_DECLARATION
#include "ex3_asm_def.h"
#undef INSN_FUNC_DECLARATION

	EX3_InsnSet(int icount);
};