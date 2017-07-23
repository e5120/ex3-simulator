#include "ex3_asmparser.h"
#include "ex3_insnset.h"

int EX3_ASMParser::ParseInsn(int passNum, const char * p, InsnSet::Insn * insn, int & addr)
{

	if (insn->type == EX3_InsnSet::REG_INSN || insn->type == EX3_InsnSet::MEM_INSN)
	{
		if (passNum == 2)
		{
			Memory::Word * m = &cpu->mem->word[addr];
			m->insnID = insn->ID;
			m->value = insn->code;
			m->SetStatus(&cpu->label.annotation, addr);
			cpu->dbg->InsertMonitorOrBreakpoint(m->status, addr, true);
			if (insn->type == EX3_InsnSet::MEM_INSN)
			{
				SkipWhiteSpace(&p);
				int len = GetLabelLength(p);
				if (len == 0)
				{
					printf("ERROR: label(%s) length = 0 (bug in parser)\n", p); PrintErrorLocation(); return -1;
				}
				Label::Element * lb = cpu->label.GetLabel(p, len);
				if (lb == 0)
				{
					printf("ERROR: label(%s) not found in the 1st pass (bug in the program...)\n", p); PrintErrorLocation(); return -1;
				}
				if (!cpu->mem->IsValidAddress(lb->address))
				{
					printf("ERROR: label(%s) has invalid address (%x)\n", lb->name, lb->address); PrintErrorLocation(); return -1;
				}
				m->value |= lb->address;
				p += lb->nlen;
				SkipWhiteSpace(&p);
				if (*p == 'I')
				{
					m->value |= 0x8000; p++;
				}	///	set bit15 to 1
			}
		}
		if (passNum == 2)
		{
			ExtractComment(p, addr, 0);
		}
		addr++;
		return 1;
	}
	else
	{
		return 0;
	}
}