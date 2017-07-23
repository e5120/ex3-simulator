#include <cstring>

using namespace std;

#include "memory.h"

Memory::Word::Word()
{
	status = MS_Null; insnID = 0; value = 0; headComment = 0; tailComment = 0;
}

Memory::Word::~Word()
{
	if (headComment)
	{
		delete[] headComment;
	}
	if (tailComment)
	{
		delete[] tailComment;
	}
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

void Memory::Word::SetComment(const char * c, int len, int headFlag)
{
	if (headFlag)
	{
		int len0 = (headComment) ? strlen(headComment) + 2 : 0;
		int len2 = len0 + len;
		char * comment2 = new char[len2 + 1];
		char * p = comment2;
		if (headComment)
		{
			sprintf(p, "%s\n/", headComment); p += strlen(headComment) + 2;
		}
		strncpy(p, c, len);
		comment2[len2] = 0;
		if (headComment)
		{
			delete[] headComment;
		}
		headComment = comment2;
	}
	else if (tailComment == 0)
	{
		char * comment2 = new char[len + 1];
		char * p = comment2;
		strncpy(p, c, len);
		comment2[len] = 0;
		tailComment = comment2;
	}
}

Memory::Memory(int sz)
{
	size = sz;
	maxAddr = 0;
	errorFlag = 0;
	curCode = 0;
	word = new Word[size];
}

Memory::~Memory()
{
	delete[] word;
}

bool Memory::IsValidAddress(int addr)
{
	return addr >= 0 && addr < size;
}

Memory::Word * Memory::GetWord(unsigned int addr)
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
	pc++;
}

unsigned short Memory::Address()
{
	Word * m = (IsIndirectMemoryAccess(curCode)) ? GetWord(GetAddressField(curCode)) : curCode;
	return GetAddressField(m);
}

unsigned short & Memory::Operand()
{
	return GetWord(Address())->value;
}
