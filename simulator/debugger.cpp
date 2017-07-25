#include "debugger.h"
#include "utils.h"

void Debugger::Breakpoint::Set(int id, BreakpointType bt, int value, unsigned short * mem0)
{
	ID = id, btype = bt;
	mem = 0; val = 0;
	if (btype == BT_PC)
	{
		addr = value;
	}
	else if (btype == BT_ICOUNT)
	{
		val = value;
	}
	else
	{
		addr = value; mem = mem0;
	}
}
const char * Debugger::Breakpoint::GetBreakpointTypeName()
{
	switch (btype)
	{
	case BT_PC:		return "PC";
	case BT_ICOUNT:	return "ICOUNT";
	case BT_MEM:	return "MEM";
	}
}
void Debugger::Breakpoint::PrintInfo(Debugger * dbg)
{
	printf("Breakpoint(%d) at ", ID);
	if (btype == BT_PC)
	{
		printf("PC (0x%03x)", addr);
	}
	else if (btype == BT_ICOUNT)
	{
		printf("insnCount (%d)", val);
	}
	else
	{
		printf("MEM (0x%03x) = (%04x : %04x)", addr, val, *mem);
	}
	if (dbg)
	{
		printf("\n"); dbg->ShowCPUStatus();
	}
	else
	{
		printf("\n");
	}
}
Debugger::Debugger(CPU * cpu0)
{
	if (globalDBG)
	{
		ABORT_PROGRAM(printf("globalDBG != 0\n"));
	}
	globalDBG = this;
	cpu = cpu0; breakpointCount = 0; monitorCount = 0; bpID = 0; com = CT_Start; verboseMode = 0; fileLogMode = 0;
}
Debugger::~Debugger()
{
	globalDBG = 0;
}
bool Debugger::Echo()
{
	return (com == CT_Step || verboseMode || DetectMonitor());
}
bool Debugger::FileLog()
{
	return (com == CT_Start || fileLogMode || DetectMonitor());
}
void Debugger::ShowCommand()
{
	printf("r : run(server)\n"
		"c : run(client)\n"
		"s : step\n"
		"a : output mem,prb file\n"
		"b : show breakpoints\n"
		"d : delete breakpoint\n"
		"p : set PC breakpoint\n"
		"i : set InsnCount breakpoint\n"
		"m : set Memory Monitor breakpoint\n"
		"l : toggle verbose CPU-log mode (verboseMode = %d)\n"
		"f : toggle file CPU-log mode (fileLogMode = %d)\n"
		"w : show CPU status\n"
		"n : show PC monitors\n"
		"h : show commands\n"
		"q : quit\n"
		, verboseMode, fileLogMode
	);
}
int Debugger::Debug()
{
	UpdateMonitor();
	switch (com)
	{
	case CT_Run: if (!DetectBreakpoint())	return 0;
	case CT_Step:							break;
	case CT_Start: ShowCommand();			break;
	}
	return Command();
}
int Debugger::Command()
{
	while (1)
	{
		printf("EX3-DBG > ");
		ch = _getche();
		printf("\n");
		switch (ch)
		{
		case 'r': com = CT_Run;						return 0;
        case 'c': com = CT_Run;                     return 0;
		case '\n':
		case 's': com = CT_Step;					return 0;
		case 'a': return 1;
		case 'b': ShowBreakpoints();				break;
		case 'd': DeleteBreakpoint();				break;
		case 'p': InsertBreakpoint(BT_PC);			break;
		case 'i': InsertBreakpoint(BT_ICOUNT);		break;
		case 'm': InsertBreakpoint(BT_MEM);			break;
		case 'n': ShowMonitors();					break;
		case 'l': ToggleVerboseMode();				break;
		case 'f': ToggleFileLogMode();				break;
		case 'w': ShowCPUStatus();					break;
		case 'h': ShowCommand();					break;
		case 'q': return 1;
		default: ShowCommand();					break;
		}
	}
	return 0;
}
void Debugger::ToggleVerboseMode()
{
	verboseMode = 1 - verboseMode; printf("verboseMode = %d\n", verboseMode);
}
void Debugger::ToggleFileLogMode()
{
	fileLogMode = 1 - fileLogMode; printf("fileLogMode = %d\n", fileLogMode);
}
void Debugger::EnableAllLogs()
{
	verboseMode = 1; fileLogMode = 1;
}
void Debugger::ShowBreakpoints()
{
	printf("%d breakpoints\n", breakpointCount);
	int i;
	for (i = 0; i < breakpointCount; i++)
	{
		breakpoints[i].PrintInfo();
	}
}
void Debugger::GetValue(const char * msg, const char * field, int * value)
{
	printf("%s", msg);
	scanf(field, value);
}
void Debugger::DeleteBreakpoint()
{
	int ID;
	GetValue("delete breakpoint ID = ", "%d", &ID);
	int i;
	for (i = 0; i < breakpointCount; i++)
	{
		if (breakpoints[i].ID == ID)
		{
			breakpointCount--;
			for (; i < breakpointCount; i++)
			{
				breakpoints[i] = breakpoints[i + 1];
			}
			break;
		}
	}
	ShowBreakpoints();
}
void Debugger::InsertBreakpoint(BreakpointType bt)
{
	if (breakpointCount >= BREAKPOINT_COUNT)
	{
		printf("Too many breakpoints... (delete some to add new ones)\n"); return;
	}
	int value;
	unsigned short * mem0 = 0;
	if (bt == BT_ICOUNT)
	{
		GetValue("insn count (decimal) = ", "%d", &value);
	}
	else
	{
		GetValue("address (hex) = ", "%x", &value);
		if (value >= cpu->mem->size)
		{
			printf("address is out of range...\n"); return;
		}
		if (bt == BT_MEM)
		{
			mem0 = &cpu->mem->word[value].value;
		}
	}
	breakpoints[breakpointCount++].Set(bpID++, bt, value, mem0);
	ShowBreakpoints();
}
void Debugger::InsertMonitorOrBreakpoint(int status, int addr, bool isInsn)
{
	if (addr < 0 || addr >= cpu->mem->size)
	{
		ABORT_PROGRAM(printf("Error in InsertMonitorOrBreakpoint!!!\n"));
	}
	BreakpointType bt = (isInsn) ? BT_PC : BT_MEM;
	unsigned short * mem0 = 0;
	if (bt == BT_MEM)
	{
		mem0 = &cpu->mem->word[addr].value;
	}
	if (status & Memory::MS_Monitor)
	{
		if (monitorCount >= MONITOR_COUNT)
		{
			printf("[InsertMonitorOrBreakpoint] Too many monitors... \n"); return;
		}
		monitors[monitorCount].Set(monitorCount, bt, addr, mem0);
		monitorCount++;
	}
	if (status & Memory::MS_Breakpoint)
	{
		if (breakpointCount >= BREAKPOINT_COUNT)
		{
			printf("[InsertMonitorOrBreakpoint] Too many breakpoints... (delete some to add new ones)\n"); return;
		}
		breakpoints[breakpointCount++].Set(bpID++, bt, addr, mem0);
	}
}
bool Debugger::DetectBreakpoint()
{
	int i;
	Breakpoint * bp = breakpoints;
	for (i = 0; i < breakpointCount; i++, bp++)
	{
		switch (bp->btype)
		{
		case BT_PC:		if (cpu->_PC == bp->addr)
		{
			bp->PrintInfo(this); return true;
		} break;
		case BT_ICOUNT:	if (cpu->cycle_count == bp->val)
		{
			bp->PrintInfo(this); return true;
		} break;
		case BT_MEM:	if (*bp->mem != bp->val)
		{
			bp->PrintInfo(this); bp->val = *bp->mem; return true;
		} break;
		}
	}
	return false;
}
void Debugger::ShowMonitors()
{
	int i;
	Breakpoint * mon = monitors;
	printf("monitors = {");
	for (i = 0; i < monitorCount; i++, mon++)
	{
		if (i)
		{
			printf(", ");
		}
		printf("0x%03x(%s)", mon->addr, (mon->btype == BT_PC) ? "PC" : "MEM");
	}
	printf("}\n");
}
bool Debugger::DetectMonitor()
{
	int i;
	Breakpoint * mon = monitors;
	for (i = 0; i < monitorCount; i++, mon++)
	{
		switch (mon->btype)
		{
		case BT_PC:		if (cpu->_PC == mon->addr)
		{
			return true;
		} break;
		case BT_MEM:	if (*mon->mem != mon->val)
		{
			return true;
		} break;
        default:    break;
		}
	}
	return false;
}
void Debugger::UpdateMonitor()
{
	int i;
	Breakpoint * mon = monitors;
	for (i = 0; i < monitorCount; i++, mon++)
	{
		if (mon->mem)
		{
			mon->val = *mon->mem;
		}
	}
}
void Debugger::ShowCPUStatus()
{
	cpu->PrintStatus(stdout, 0);
}
