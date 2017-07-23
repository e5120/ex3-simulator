#pragma once

#include "system.h"
#include "ex3_cpu.h"
#include "ex3_asmparser.h"

class EX3_TerminalSystem
{
public:
	EX3_CPU cpu;
	EX3_ASMParser parser;
	System::TerminalViewer termView;
	System::InputTerminal inTerm;
	System::OutputTerminal outTerm;

	EX3_TerminalSystem(const char * fname);
	~EX3_TerminalSystem();

	void AccessInPort();
	void AccessOutPort();

	void PrintString(const char * s);

	void Close();

	void RunCPUModel();
};