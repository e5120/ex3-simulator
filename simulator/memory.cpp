#include "memory.h"

Memory::Word::Word() : status(MS_Null), insnID(0), value(0), headComment(), tailComment()
{
}

Memory::Word::~Word()
{
}

void Memory::Word::SetStatus(Label::AnnotationStatus * labelAnnotation, int addr)
{
	status = MS_Used;
	if (labelAnnotation->curAddr == addr)
	{
		if (labelAnnotation->annotation & Label::AL_Monitor)
		{
			status |= MS_Monitor;
		}
		if (labelAnnotation->annotation & Label::AL_Breakpoint)
		{
			status |= MS_Breakpoint;
		}
	}
}

void Memory::Word::SetComment(const std::string& c, int len, int headFlag)
{
	if (headFlag)
	{
        headComment += c.substr(0, len);
	}
	else if (tailComment.empty())
	{
        tailComment = c.substr(0, len);
	}
}

Memory::Memory(int sz) : word(sz), size(sz), maxAddr(0), errorFlag(0)
{
}

Memory::~Memory()
{
}

bool Memory::IsValidAddress(int addr)
{
	return addr >= 0 && addr < size;
}

Memory::Word* Memory::GetWord(unsigned int addr)
{
	if (!IsValidAddress(addr) || errorFlag)
	{
		errorFlag = 1;
		return &bogusWord;
	}
	word[addr].status |= MS_Used;
	return &word[addr];
}

void Memory::ProbeInsn(unsigned short pc)
{
	curCode = GetWord(pc);
}

void Memory::FetchInsn(unsigned short & pc)
{
	curCode = GetWord(pc);
	++pc;
}

unsigned short Memory::Address()
{
	Word* m = (IsIndirectMemoryAccess(curCode)) ? GetWord(GetAddressField(curCode)) : curCode;
	return GetAddressField(m);
}

unsigned short Memory::Operand()
{
	return GetWord(Address())->value;
}
