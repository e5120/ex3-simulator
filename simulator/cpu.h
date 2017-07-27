#pragma once
#include "utils.h"
#include "defs.h"
#include "label.h"
#include "memory.h"
#include "insnset.h"

class Debugger;
#include "debugger.h"

class CPU
{
public:
	Label label;
	std::shared_ptr<Memory> mem;
	std::shared_ptr<InsnSet> isa;
	unsigned short _S, _PC, _oldPC, _INPR, _OUTR, _FGI, _FGO;
	int cycle_count, appl_cycle_count, intr_cycle_count, intr_pending;
	std::shared_ptr<FILE> fplog;
	std::shared_ptr<Debugger> dbg;

	CPU(const std::string& logfilename);
	virtual ~CPU();

	void PrintSeparator(FILE* fp);

	virtual void Reset();

	void PrintMemory(FILE* fp, int printMode);

	bool IsInputReady();
	bool IsOutputReady();

	void SetInput(unsigned char val);
	unsigned char GetOutput();

	///	virtual abstract functions : must be defined in actual class
	virtual int GetSelectedPortID() = 0;
	virtual int Execute() = 0;
	virtual bool IsWaitingForInput() = 0;
	virtual void _SetFGI(int val) = 0;
	virtual void _SetFGO(int val) = 0;
	virtual int _GetFGI() = 0;
	virtual int _GetFGO() = 0;
	virtual	void PrintStatus(FILE* fp, bool intr_cycle) = 0;
    virtual void PrintMemoryWord(Memory::Word* m, FILE* fp, int addr, int printMode) = 0;

	enum PrintMemoryWordMode
	{
		PM_Null = 0x0,
		PM_Verilog = 0x1,
		PM_Program = 0x2,
		PM_Monitor = 0x4,
		PM_SkipHeadComment = 0x8,
	};
};