#pragma once

#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

class EX3_CPU : public CPU
{
public:
	unsigned short _AC, _E, _R, _IEN, _IMSK, _IOT, inputPending, _TF, _TMRF;
    double _TIMER;
	EX3_CPU(int mem_size);

	void Reset();

	virtual int GetSelectedPortID();

	virtual bool IsWaitingForInput();

	void SetFG(unsigned short * fg, int val);

	virtual void _SetFGI(int val);
	virtual void _SetFGO(int val);

	virtual int _GetFGI();
	virtual int _GetFGO();

	void PrintDataMemory();
	void PrintStatus(FILE * fp, bool intr_cycle);
	int Execute();
	void PrintMemoryWord(Memory::Word * m, FILE * fp, int addr, int printMode);
};
