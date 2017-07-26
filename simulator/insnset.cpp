#include "insnset.h"

InsnSet::Insn::Insn() : ID(0), type(0), nlen(0), showMemFlag(0), name(), operation(OP_DUMMY)
{
}

InsnSet::Insn::~Insn()
{
}

void InsnSet::Insn::Set(int id, const std::string& n, int t, int showMem, std::function<void(CPU*)> op, unsigned short c)
{
	ID = (unsigned char)id;
	name = n;
	type = (unsigned char)t;
	showMemFlag = (unsigned  char)showMem;
	nlen = (unsigned char)name.size();
	operation = op;
	code = c;
}

InsnSet::Insn InsnSet::SearchInsn(std::string& iname_p)
{
	for (int i = 0; i < ICount; ++i)
	{
		Insn ii = insn[i];
        if (ii.nlen > 0 && ii.name.substr(0,ii.nlen) == iname_p.substr(0,ii.nlen))
        {
            iname_p.erase(0,ii.nlen);
			return ii;
		}
	}
	Insn null;
	return null;
}
InsnSet::InsnSet(int icount) : insn(icount), ICount(icount)
{
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
