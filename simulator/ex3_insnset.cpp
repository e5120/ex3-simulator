#include "ex3_insnset.h"
#include "ex3_cpu.h"

EX3_InsnSet::EX3_InsnSet(int icount) : InsnSet(icount)
{
    srand((unsigned int)time(NULL));
#define INSN_CREATE
#include "ex3_asm_def.h"
#undef INSN_CREATE
}

#define INSN_FUNC_DEFINITION
#include "ex3_asm_def.h"
#undef INSN_FUNC_DEFINITION