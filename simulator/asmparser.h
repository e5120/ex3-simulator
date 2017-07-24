#pragma once
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>

#include "cpu.h"

class ASMParser
{
public:
	std::shared_ptr<CPU> cpu;
	FILE * fp;
	const std::string asm_name;
	static std::string tool_name;
	int asmLineNum, blockCommentPending;

	ASMParser(const std::shared_ptr<CPU>& cpu0, const std::string fname);
	~ASMParser();

	int Open();
	void Close();

	FILE * OpenFile(const std::string extname);

	int WriteVerilogMemFile();
	int WriteVerilogMemProbeFile();
	int WriteVerilogMonitorFile();

	void SkipWhiteSpace(std::string& pp);

	int GetNum(std::string& pp, int insnType);

	int GetLabelLength(std::string& p);

	int ExtractComment(std::string& p, int addr, int headFlag);
	void PrintErrorLocation();
	int ParseLabel(std::string& p);
	int ParseLabel(int passNum, std::string& p, int& addr);
	int ParseNonInsn(int passNum, std::string& p, int insnID, int & addr);
	
	int ParseBlockCommentStart(std::string& p);
	int ParseBlockCommentEnd(std::string& p);
	int ParseBlockComment(std::string& p);
	int Parse(int passNum);

	///	virtual abstract functions : must be defined in actual class
	virtual int ParseInsn(int passNum, std::string& p, InsnSet::Insn * insn, int& addr) = 0;
};