#pragma once

class CPU;

class InsnSet
{
public:
	class Insn
	{
	public:
		unsigned char ID;
		unsigned char type;
		unsigned char nlen;
		unsigned char showMemFlag;
		const char * name;
		unsigned short code;
		void(*operation)(CPU *);
		Insn();
		void Set(int id, const char * n, int t, int showMem, void(*op)(CPU *), unsigned short c);
	};

	Insn * SearchInsn(const char ** iname_p);
	Insn * insn;
	int ICount;
	
	enum InsnID
	{
		I_INVALID, /// valid InsnID is non-zero
		I_ORG, I_END, I_DEC, I_HEX, I_CHR, I_SYM,
		I_TAIL,
	};

	InsnSet(int icount);
	~InsnSet();
	static void OP_DUMMY(CPU * cpu)
	{}
};