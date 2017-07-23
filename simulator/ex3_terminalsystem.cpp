#include "ex3_terminalsystem.h"

EX3_TerminalSystem::EX3_TerminalSystem(const char * fname) : cpu(0x1000), parser(&cpu, fname), inTerm(&cpu), outTerm(&cpu)
{
	inTerm.Open(parser.OpenFile("_in.log"));
	outTerm.Open(parser.OpenFile("_out.log"));
	inTerm.termView = &termView;
	outTerm.termView = &termView;
}

EX3_TerminalSystem::~EX3_TerminalSystem()
{
	cpu.dbg->EnableAllLogs();
	Close();
}

void EX3_TerminalSystem::AccessInPort()
{
	inTerm.AccessPort();
	cpu._FGI = 0;
}

void EX3_TerminalSystem::AccessOutPort()
{
	cpu._FGO = 0;
	outTerm.AccessPort();
}

void EX3_TerminalSystem::PrintString(const char * s)
{
	while (*s)
	{
		cpu._OUTR = *(s++);
		AccessOutPort();
	}
}

void EX3_TerminalSystem::Close()
{
	cpu.PrintDataMemory();
	parser.Close();
}

void EX3_TerminalSystem::RunCPUModel()
{
	cpu.Reset();
	int inFlag, outFlag, cpuFlag;
#if SOCKET
    if(System::RandomPeripheral::SocketStart()){
        inTerm.Close();
        outTerm.Close();
        termView.PrintView();
        return;
    }
#endif
    do
	{
		if (cpu.dbg->Debug())
		{
			break;
		}
		///	1. simulate components : inTerm, outTerm, cpu
		inFlag = inTerm.Execute();
		outFlag = outTerm.Execute();
		cpuFlag = cpu.Execute();
	} while (inFlag + cpuFlag + outFlag == 0);
	inTerm.Close();
	outTerm.Close();
	termView.PrintView();
}
