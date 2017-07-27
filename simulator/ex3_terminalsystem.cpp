#include "ex3_terminalsystem.h"

EX3_TerminalSystem::EX3_TerminalSystem(const std::string& fname)
		: cpu(new EX3_CPU(0x1000)), parser(cpu, fname),
          termView(new System::TerminalViewer()), inTerm(cpu), outTerm(cpu)
{
	inTerm.Open(parser.OpenFile("_in.log"));
	outTerm.Open(parser.OpenFile("_out.log"));

   	inTerm.termView = termView;
    outTerm.termView = inTerm.termView;
}

EX3_TerminalSystem::~EX3_TerminalSystem()
{
	cpu->dbg->EnableAllLogs();
	Close();
}

void EX3_TerminalSystem::AccessInPort()
{
	inTerm.AccessPort();
	cpu->_FGI = 0;
}

void EX3_TerminalSystem::AccessOutPort()
{
	cpu->_FGO = 0;
	outTerm.AccessPort();
}

void EX3_TerminalSystem::PrintString(const std::string& s)
{
    for(int i=0;i<s.size();++i){
        cpu->_OUTR = s[i];
        AccessOutPort();
    }
}

void EX3_TerminalSystem::Close()
{
	cpu->PrintDataMemory();
	parser.Close();
}

void EX3_TerminalSystem::RunCPUModel()
{
	cpu->Reset();
	int inFlag, outFlag, cpuFlag;
#if SOCKET
    if(System::RandomPeripheral::SocketStart()){
        inTerm.Close();
        outTerm.Close();
        termView->PrintView();
        return;
    }
#endif
    do
	{
		if (cpu->dbg->Debug())
		{
			break;
		}
		cpu->dbg->Echo();
		cpu->dbg->FileLog();

		///	1. simulate components : inTerm, outTerm, cpu
		inFlag = inTerm.Execute();
		outFlag = outTerm.Execute();
		cpuFlag = cpu->Execute();
	} while (inFlag + cpuFlag + outFlag == 0);
	inTerm.Close();
	outTerm.Close();
	termView->PrintView();
}
