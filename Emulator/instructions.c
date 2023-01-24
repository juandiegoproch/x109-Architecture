#include <stdint.h>
#include "declarations.h"

// MLOAD defines
#define MLOAD_CONFIG_MSB_MASK 		0b0000010000000000
#define MLOAD_CONFIG_STACKOP_MASK	0b0000001000000000
#define MLOAD_CONFIG_STORE_MASK 	0b0000100000000000
#define MLOAD_ADDRESSING_MASK		0b0000000100000000
#define NEGATIVE_BIT_8 0b10000000

#define CLR_MSB_MASK 0b0000000011111111
#define CLR_LSB_MASK 0b1111111100000000

#define MLOAD_IMMEDIATEV_MASK  0b0000000011111111
#define MSTACK_IMMEDIATEV_MASK 0b0000000000001111

// ALUOP defines

#define ACCOUNTNEG_MASK 0b0000000000000010
#define ACCOUNTNEG_OFFSET 1

#define ACCOUNTZ_MASK 0b0000000000000001
#define ACCOUNTZ_OFFSET 0

#define ALUCFG_MASK 0b0000111100000000
#define ALUCFG_OFFSET 8

void executeAluOpInstruction(uint16_t instruction)
{
	// Decode the instruction's arguments
	// flags for jump
	bool accountNegative = (instruction & ACCOUNTNEG_MASK) >> ACCOUNTNEG_OFFSET;
	bool accountZero     = (instruction & ACCOUNTZ_MASK)   >> ACCOUNTZ_OFFSET  ;
	
	// Resgisters
	uint8_t operand1Register = decodeOp1reg(instruction);
	uint8_t operand2Register = decodeOp2reg(instruction);
	uint8_t storeRegister = decodeStoreReg(instruction);
	// ALU configuration
	uint8_t aluConfiguration = (instruction & ALUCFG_MASK) >> ALUCFG_OFFSET;
	
	// Execute the ALU operation
	// Fetch register VALUES
	uint16_t operand1Value = registerop(operand1Register,'r',0);
	uint16_t operand2Value = registerop(operand2Register,'r',0);
	
	// Perform the operation
	uint16_t operationResult = aluOperation(aluConfiguration,operand1Value,operand2Value);
	
	// Check for branch condition
	
	if ( accountNegative || accountZero) // If any flag is not ignored
	{
		if ((accountNegative && negativeFlag) || (accountZero && zeroFlag)) // if any of the flags getting considered is true jump
		{
			uint16_t jumpto = registerop(JR,'r',0);
			registerop(PC,'w',jumpto);
		}
	}	
	else
	{
		registerop(storeRegister,'w',operationResult); // if no flag just store
	}
	
	
	
}

void executeMLoadInstruction(uint16_t instruction)
{
	bool msb 					= (bool)(instruction & MLOAD_CONFIG_MSB_MASK);
	bool store 					= (bool)(instruction & MLOAD_CONFIG_STORE_MASK);
	bool indirect_addressing	= (bool)(instruction & MLOAD_ADDRESSING_MASK);
	bool stack_operation 		= (bool)(instruction & MLOAD_CONFIG_STACKOP_MASK);

	uint8_t operand_register = decodeStoreReg(instruction);
	uint8_t immediate_val = (instruction & MLOAD_IMMEDIATEV_MASK);
	uint8_t second_nibble_immediateval = (instruction & MSTACK_IMMEDIATEV_MASK);

	if (stack_operation && !indirect_addressing)
	{
		// we have a stack operation
		uint16_t spval = registerop(SP,'r',0);
		if (store)
		{
			// Push
			registerop(SP,'w',--spval);
			memory[spval] = registerop(operand_register,'r',0) + second_nibble_immediateval;  
			return;
		}
		else
		{
			// Pop
			registerop(operand_register,'w',memory[spval] + second_nibble_immediateval);
			registerop(SP,'w',++spval);
			return;
		}
	} 
	else if (indirect_addressing) // Memory Operation 128W up/down
	{
		// is negative
		uint16_t memory_address;
		if (immediate_val & NEGATIVE_BIT_8)
		{
			immediate_val = ~immediate_val; // two's compliment wizardry
			immediate_val++;

			memory_address = (stack_operation?registerop(ML,'r',0):registerop(ML,'r',0)) - immediate_val;
		}
		else
		{
			memory_address = (stack_operation?registerop(ML,'r',0):registerop(ML,'r',0)) + immediate_val;
		}


		if (store)
		{
			//Store
			memory[memory_address] = registerop(operand_register,'r',0);
			return;
		}
		else
		{
			//Load
			registerop(operand_register,'w',memory[memory_address]);
			return;
		}
	}

	uint16_t valueToStore = immediate_val << (msb*8); // Calculate if MSB should be set
	uint16_t oldvalue = registerop(operand_register,'r',0); // get the old value
	if (msb) oldvalue &= CLR_MSB_MASK; else oldvalue&= CLR_LSB_MASK;
	valueToStore |= oldvalue;
	registerop(operand_register,'w',valueToStore);
}

uint16_t fetchInstruction()
{
		uint16_t adressOfInstruction = registerop(PC,'r',0);
		uint16_t currentInstruction = memory[adressOfInstruction]; // load the current instruction as the instruction currently pointed to by PC
		uint16_t nextInstrAdress = adressOfInstruction + 1;
		registerop(PC,'w', nextInstrAdress); // increment the program counter
		
		return currentInstruction;
}

