#pragma once

#include <cstdio>

#include "cpu.h"

class ASMParser
{
public:
	CPU * cpu;
	FILE * fp;
	const char * asm_name;
	static const char * tool_name;
	int asmLineNum, blockCommentPending;

	ASMParser(CPU * cpu0, const char * fname);
	~ASMParser();

	int Open();
	void Close();

	FILE * OpenFile(const char * extname);

	int WriteVerilogMemFile();
	int WriteVerilogMemProbeFile();
	int WriteVerilogMonitorFile();

	void SkipWhiteSpace(const char ** pp);

	int GetNum(const char ** pp, int insnType);

	int GetLabelLength(const char * p);

	int ExtractComment(const char * p, int addr, int headFlag);
	void PrintErrorLocation();
	int ParseLabel(const char * p);
	int ParseLabel(int passNum, const char ** p, int & addr);
	int ParseNonInsn(int passNum, const char * p, int insnID, int & addr);
	
	int ParseBlockCommentStart(const char ** p);
	int ParseBlockCommentEnd(const char ** p);
	int ParseBlockComment(const char ** p);
	int Parse(int passNum);

	///	virtual abstract functions : must be defined in actual class
	virtual int ParseInsn(int passNum, const char * p, InsnSet::Insn * insn, int & addr) = 0;
};