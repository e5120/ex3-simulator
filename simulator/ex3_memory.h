#pragma once

#include "memory.h"

class EX3_Memory : public Memory
{
public:
	EX3_Memory(int sz);
	virtual ~EX3_Memory();

	bool IsIndirectMemoryAccess(Word* w);
	unsigned short GetAddressField(Word* m);
};