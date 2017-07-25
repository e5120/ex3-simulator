#include "ex3_memory.h"

EX3_Memory::EX3_Memory(int sz) : Memory(sz)
{
}

EX3_Memory::~EX3_Memory()
{
}

bool EX3_Memory::IsIndirectMemoryAccess(Word* w)
{
	return (w->value & 0x8000) != 0;
}

unsigned short EX3_Memory::GetAddressField(Word* m)
{
	return m->value & 0xfff;
}