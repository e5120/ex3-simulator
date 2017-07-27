#pragma once
#include <string>
#include <vector>
#include "label.h"

class Memory
{
public:
	enum Status
	{	///	bit[0] : 1 (used as instruction or data), 0 (unused)
		MS_Null = 0x0,	///	00
		MS_Used = 0x1,	///	01
		MS_Monitor = 0x2,	///	for ISS and Verilog simulations
		MS_Breakpoint = 0x4,	///	for ISS (breakpoint set)
	};

	class Word
	{
	public:
		unsigned char status;
		unsigned char insnID;
		unsigned short value;
		std::string headComment;
		std::string tailComment;

		Word();
		virtual ~Word();

		void SetStatus(const Label::AnnotationStatus& labelAnnotation, int addr);
		void SetComment(const std::string& c, int len, int headFlag);
	};

    std::vector<Word> word;
    Word* curCode;
    Word bogusWord;	    /// for returning memory word upon access error
	int size;
    int maxAddr;
	unsigned short errorFlag;

	Memory(int sz);
	virtual ~Memory();

	bool IsValidAddress(int addr);

	Word* GetWord(unsigned int addr);
	void ProbeInsn(unsigned short pc);
	void FetchInsn(unsigned short& pc);

	unsigned short Address();
	unsigned short Operand();

	///	virtual abstract functions : must be defined in actual class
	virtual bool IsIndirectMemoryAccess(Word* w) = 0;
	virtual unsigned short GetAddressField(Word* m) = 0;
};
