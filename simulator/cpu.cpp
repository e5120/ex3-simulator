#include "cpu.h"
#include "memory.h"

CPU::CPU(const char * logfilename) : mem(0), isa(0), dbg(0)
{
	fplog = fopen(logfilename, "w");
	Reset();
}

CPU::~CPU()
{
	if (mem)
	{
		delete mem;
	}
	if (isa)
	{
		delete isa;
	}
	if (fplog)
	{
		fclose(fplog);
	}
}

void CPU::PrintSeparator(FILE * fp)
{
	fprintf(fp, "----------------\n");
}

void CPU::Reset()
{
	cycle_count = 0;
	appl_cycle_count = 0;
	intr_cycle_count = 0;
	intr_pending = 0;
	_S = 1;
	_oldPC = PROGRAM_ENTRY_POINT;
	_PC = PROGRAM_ENTRY_POINT;
	_INPR = 0;
	_OUTR = 0;
	_FGI = 0x0;
	_FGO = 0x3;
}

void CPU::PrintMemory(FILE * fp, int printMode)
{
	int i;
	for (i = 0; i < mem->maxAddr; i++)
	{
		PrintMemoryWord(&mem->word[i], fp, i, printMode);
	}
}

bool CPU::IsInputReady()
{
	return _GetFGI() == 0;
}

bool CPU::IsOutputReady()
{
	return _GetFGO() == 0;
}
void CPU::SetInput(unsigned char val)
{
	if (IsInputReady())
	{
		_SetFGI(1);
		_INPR = val;
	}
	else
	{
		printf("ERROR!!! SetInput() called when input is not ready...\n");
	}
}

unsigned char CPU::GetOutput()
{
	if (IsOutputReady())
	{
		_SetFGO(1); return (unsigned char)_OUTR;
	}
	else
	{
		printf("ERROR!!! GetInput() called when output is not ready...\n"); return -1;
	}
}
