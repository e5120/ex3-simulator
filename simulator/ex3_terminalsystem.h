#pragma once

#include "system.h"
#include "ex3_cpu.h"
#include "ex3_asmparser.h"

class EX3_TerminalSystem
{
public:
	std::shared_ptr<EX3_CPU> cpu;
	EX3_ASMParser parser;
	System::TerminalViewer termView;
	System::InputTerminal inTerm;
	System::OutputTerminal outTerm;

	EX3_TerminalSystem(const std::string& fname);
	virtual ~EX3_TerminalSystem();

	void AccessInPort();
	void AccessOutPort();
	void PrintString(const std::string& s);
	void Close();
	void RunCPUModel();
};