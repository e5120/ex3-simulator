#include "ex3_asmparser.h"

EX3_ASMParser::EX3_ASMParser(const std::shared_ptr<CPU>& cpu0, const std::string& fname)
		: ASMParser(cpu0, fname)
{
}

int EX3_ASMParser::ParseInsn(int passNum, std::string& p, const InsnSet::Insn& insn, int addr)
{
	if (insn.type == EX3_InsnSet::REG_INSN || insn.type == EX3_InsnSet::MEM_INSN)
	{
		if (passNum == 2)
		{
			Memory::Word m = cpu->mem->word[addr];
			m.insnID = insn.ID;
			m.value = insn.code;
			m.SetStatus(&cpu->label.annotation, addr);
			cpu->dbg->InsertMonitorOrBreakpoint(m.status, addr, true);
			if (insn.type == EX3_InsnSet::MEM_INSN)
			{
				SkipWhiteSpace(p);
				int len = GetLabelLength(p);
				if (len == 0)
				{
                    cpu->mem->word[addr] = m;
                    printf("ERROR: label(%s) length = 0 (bug in parser)\n", p.c_str());
                    PrintErrorLocation();
                    return -1;
				}
				Label::Element lb = cpu->label.GetLabel(p, len);
				if (lb.name.empty())
				{
                    cpu->mem->word[addr] = m;
                    printf("ERROR: label(%s) not found in the 1st pass (bug in the program...)\n", p.c_str());
                    PrintErrorLocation();
                    return -1;
				}
				if (!cpu->mem->IsValidAddress(lb.address))
				{
                    cpu->mem->word[addr] = m;
                    printf("ERROR: label(%s) has invalid address (%x)\n", lb.name.c_str(), lb.address);
                    PrintErrorLocation();
                    return -1;
				}
				m.value |= lb.address;
				p.erase(0, (unsigned long)lb.nlen);
				SkipWhiteSpace(p);
				if (p.front() == 'I')
				{
					m.value |= 0x8000;
                    p.erase(p.begin());
				}	///	set bit15 to 1
			}
			cpu->mem->word[addr] = m;
		}
		if (passNum == 2)
		{
			ExtractComment(p, addr, 0);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}