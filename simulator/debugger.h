#pragma once
#include <memory>

class CPU;
#include "cpu.h"

class Debugger
{
public:
	enum BreakpointType
	{
		BT_PC,		///	break at PC
		BT_ICOUNT,	/// break at insnCount
		BT_MEM,		///	break at memory change
	};

	class Breakpoint
	{
	public:
		BreakpointType btype;
		int val;
		int ID;
		int addr;
		//std::unique_ptr<unsigned  short> mem;
		unsigned short * mem;

		Breakpoint();
		virtual ~Breakpoint();

		void Set(int id, BreakpointType bt, int value, unsigned short * mem0);
		const char * GetBreakpointTypeName();

		void PrintInfo(Debugger * dbg = 0);
	};

	enum CommandType
	{
		CT_Start,				/// initial state
		CT_Run,					/// 'r'
		CT_Step,				/// 's'
	};

#define BREAKPOINT_COUNT 100
#define MONITOR_COUNT 100

	Breakpoint breakpoints[BREAKPOINT_COUNT], monitors[MONITOR_COUNT];
	int breakpointCount, monitorCount, bpID;
	int verboseMode, fileLogMode;
	int ch;
	CommandType com;
	CPU * cpu;
	static Debugger * globalDBG;

	Debugger(CPU * cpu0);

	~Debugger();

	bool Echo();

	bool FileLog();
	void ShowCommand();
	int Debug();
	int Command();
	void ToggleVerboseMode();
	void ToggleFileLogMode();
	void EnableAllLogs();
	void ShowBreakpoints();
	void GetValue(const char * msg, const char * field, int * value);
	void DeleteBreakpoint();
	void InsertBreakpoint(BreakpointType bt);
	void InsertMonitorOrBreakpoint(int status, int addr, bool isInsn);
	bool DetectBreakpoint();
	void ShowMonitors();
	bool DetectMonitor();
	void UpdateMonitor();
	void ShowCPUStatus();
};