#include <cstring>

using namespace std;

#include "insnset.h"

InsnSet::Insn::Insn()
{
	ID = 0; type = 0; nlen = 0; showMemFlag = 0; name = 0; operation = OP_DUMMY;
}

void InsnSet::Insn::Set(int id, const char * n, int t, int showMem, void(*op)(CPU *), unsigned short c)
{
	ID = id; name = n; type = t; showMemFlag = showMem;
	nlen = strlen(name);
	operation = op;
	code = c;
}

InsnSet::Insn * InsnSet::SearchInsn(const char ** iname_p)
{
	const char * iname = *iname_p;
	int i;
	for (i = 0; i < ICount; i++)
	{
		Insn * ii = &insn[i];
		if (ii->nlen > 0 && strncmp(ii->name, iname, ii->nlen) == 0)
		{
			*iname_p += ii->nlen; return ii;
		}
	}
	return 0;
}
InsnSet::InsnSet(int icount)
{
	ICount = icount;
	insn = new Insn[ICount];
#define NI(i)	insn[I_##i].Set(I_##i, #i, 0, 0, OP_DUMMY, 0)
	NI(ORG);
	NI(END);
	NI(DEC);
	NI(HEX);
	NI(CHR);
	NI(SYM);
#undef  NI
}

InsnSet::~InsnSet()
{
	delete[] insn;
}
