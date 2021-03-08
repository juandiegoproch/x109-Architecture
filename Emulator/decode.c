#include <stdint.h>

#include "declarations.h"

#define STREG_MASK  0b0111000000000000
#define OP1REG_MASK 0b0000000011100000
#define OP2REG_MASK 0b0000000000011100

int decode(unsigned int instruction) // Detects if the instruction is an OP or a LD
{
	int instType = instruction >> 15; //isolate opcode
	return instType;
}
int decodeStoreReg(unsigned int instruction) //Returns register that the operation should save the reult into
{
	unsigned int reg = instruction & STREG_MASK;
	reg = reg>>12; 					//shifts all bits into the first position, extracting the registerID
	return reg;					// the same principle applies to op1reg and op2reg
}
int decodeOp1reg(unsigned int instruction) // extracts the register that the op instruction takes as first operand
{
	unsigned int reg = instruction & OP1REG_MASK;
	reg = reg>>5;
	return reg;
}
int decodeOp2reg(unsigned int instruction) // extracts register that the op instruction takes as second operand
{
	unsigned int reg = instruction & OP2REG_MASK;
	reg = reg>>2;
	return reg;
}

