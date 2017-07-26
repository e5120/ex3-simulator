#pragma once
#include <string>
#include <vector>
#include <functional>

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
		std::string name;
		unsigned short code;
		std::function<void(CPU*)> operation;

		Insn();
		virtual ~Insn();

		void Set(int id, const std::string& n, int t, int showMem, std::function<void(CPU*)> op, unsigned short c);
	};

	std::vector<Insn> insn;
	int ICount;

	InsnSet(int icount);
	virtual ~InsnSet();

	Insn SearchInsn(std::string& iname_p);
	static void OP_DUMMY(CPU* cpu){}

	enum InsnID
	{
		I_INVALID, /// valid InsnID is non-zero
		I_ORG, I_END, I_DEC, I_HEX, I_CHR, I_SYM,
		I_TAIL,
	};
};