#include "debugger.h"
#include "asmparser.h"
#include "ex3_terminalsystem.h"

bool Debugger::echo = false;
bool Debugger::flog = false;

std::string ASMParser::tool_name = "ex3_asm";

#if !defined(WIN32)
int _getche(void)
{
	GetChSetting getchSetting;
	return getchar();
}
#endif

int main(int argc, char ** argv)
{
	//#define CUR_VERSION "ver0.0 (10/05/2011)"
	//#define CUR_VERSION "ver0.1 (10/29/2012)"
	//#define CUR_VERSION "ver0.2 (12/08/2012)"
	//#define CUR_VERSION "ver0.3 (01/16/2013)"
	//#define CUR_VERSION "ver0.4 (01/17/2013)"
#define CUR_VERSION "ver0.5 (07/08/2017)"
	printf("EX3 Instruction-Set Simulator : %s\n", CUR_VERSION);
	if (argc != 2)
	{
		printf("usage: %s <asm filename>\n", argv[0]);
		printf("\nhit return : ");
		getchar();
		return -1;
	}
	ASMParser::tool_name = argv[0];
	printf("asmfile = %s\n", argv[1]);
	EX3_TerminalSystem ex3_sys(argv[1]);
	if (ex3_sys.parser.Open() != 0)
	{
		printf("ERROR during parsing\n");
		printf("\nhit return : ");
		getchar();
		return -1;
	}

	ex3_sys.RunCPUModel();

	printf("\nhit return : ");
	getchar();

	return (ex3_sys.cpu->mem->errorFlag) ? -1 : 0;
}
