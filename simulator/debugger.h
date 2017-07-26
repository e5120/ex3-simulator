#pragma once
#include <string>
#include <vector>

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
		unsigned  short mem;

		Breakpoint();
		virtual ~Breakpoint();

		void Set(int id, BreakpointType bt, int value, unsigned short mem0);
		const std::string GetBreakpointTypeName();

		void PrintInfo(Debugger* dbg = 0);
	};

	enum CommandType
	{
		CT_Start,				/// initial state
		CT_Run,					/// 'r'
		CT_Step,				/// 's'
	};

#define BREAKPOINT_COUNT 100
#define MONITOR_COUNT 100

	std::vector<Breakpoint> breakpoints, monitors;
	int breakpointCount;
    int monitorCount;
    int bpID;
	int verboseMode;
    int fileLogMode;
	int ch;
	CommandType com;
	CPU* cpu;

    static bool echo;
    static bool flog;

	Debugger(CPU* cpu0);
	virtual ~Debugger();

	void Echo();
	void FileLog();
	void ShowCommand();
	int Debug();
	int Command();
	void ToggleVerboseMode();
	void ToggleFileLogMode();
	void EnableAllLogs();
	void ShowBreakpoints();
	void GetValue(const std::string& msg, const std::string& field, int& value);
	void DeleteBreakpoint();
	void InsertBreakpoint(BreakpointType bt);
	void InsertMonitorOrBreakpoint(int status, int addr, bool isInsn);
	bool DetectBreakpoint();
	void ShowMonitors();
	bool DetectMonitor();
	void UpdateMonitor();
	void ShowCPUStatus();
};