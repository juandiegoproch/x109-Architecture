#include <stdint.h>

#include "declarations.h"

#define OPERATIONTYPE_MASK 0b00001000
#define OPERATIONTYPE_OFFSET 3

#define ZEROOPERAND_MASK   0b00000100
#define ZEROOPERAND_OFFSET   2

#define NEGATEOPERAND_MASK 0b00000010
#define NEGATEOPERAND_OFFSET 1

#define CARRYIN_MASK       0b00000001
#define CARRYIN_OFFSET      0


uint16_t aluOperation(uint8_t aluConfig,uint16_t operand1,uint16_t operand2)
{
	// Extract alu configuration flags:

	bool opType   = (aluConfig & OPERATIONTYPE_MASK) >> OPERATIONTYPE_OFFSET; 
	bool zOperand = (aluConfig & ZEROOPERAND_MASK)   >> ZEROOPERAND_OFFSET  ;
	bool nOperand = (aluConfig & NEGATEOPERAND_MASK) >> NEGATEOPERAND_OFFSET;
	bool carryIn  = (aluConfig & CARRYIN_MASK)       >> CARRYIN_OFFSET      ;
	
	// execute preprocesor on operand 1;
	
	operand1*= !zOperand; 	                 // Z config applied;
	operand1 = nOperand?~operand1:operand1; // conditional negation of operand 1
	operand1+= carryIn;                     // add carry to operand
	
	// alu operation
	uint16_t result = opType? (operand1 | operand2):(operand1+operand2); //execute logical or arit operation
	
	zeroFlag = !result; //set zero flag if result == 0
	
	negativeFlag = result>>15; // set negative flag
	
	return result;
}

