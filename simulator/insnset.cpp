#include <cstring>

using namespace std;

#include "insnset.h"

InsnSet::Insn::Insn()
{
	ID = 0;
	type = 0;
	nlen = 0;
	showMemFlag = 0;
	name = "";
	operation = OP_DUMMY;
}

void InsnSet::Insn::Set(int id, const std::string n, int t, int showMem, void(*op)(CPU *), unsigned short c)
{
	ID = id;
	name = n;
	type = t;
	showMemFlag = showMem;
	nlen = name.size();
	operation = op;
	code = c;
}

InsnSet::Insn InsnSet::SearchInsn(std::string& iname_p)
{
//	const char * iname = *iname_p;
	for (int i = 0; i < ICount; ++i)
	{
		Insn ii = insn[i];
		//printf("ii->nlen = %d\n",ii->nlen);
		if (ii.nlen > 0 && strncmp(ii.name.c_str(), iname_p.c_str(), ii.nlen) == 0)
		{
			//*iname_p += ii->nlen;
            iname_p.erase(0,ii.nlen);
			return ii;
		}
	}
	Insn dummy;
	return dummy;
}
InsnSet::InsnSet(int icount) : insn(icount)
{
	ICount = icount;
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
}
