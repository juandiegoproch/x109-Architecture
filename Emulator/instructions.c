#include <stdint.h>
#include "declarations.h"

// MLOAD defines
#define MLOAD_CONFIG_LDL 0b00000000
#define MLOAD_CONFIG_LDM 0b00000100

#define CLR_MSB_MASK 0b0000000011111111
#define CLR_LSB_MASK 0b1111111100000000

#define MLOAD_IMMEDIATEV_MASK 0b0000000011111111

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
	// flags
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

	bool msb = (instruction >> 8) & MLOAD_CONFIG_LDM; // Get if the instruction should load the msb
	uint8_t storeLocation = decodeStoreReg(instruction); // Get where to store
	uint16_t valueToStore = (instruction & MLOAD_IMMEDIATEV_MASK); //get the value o be stored
	
	valueToStore <<= (msb*8); // Shift the value to the msb if so required
	uint16_t currentRegisterValue = registerop(storeLocation,'r',0); // fetch the current stored value on the target reg
	
	if (msb) // clear the byte to be written to while preserving the other one, then store the value
	valueToStore = (currentRegisterValue & CLR_MSB_MASK) | valueToStore; 
	else
		valueToStore = (currentRegisterValue & CLR_LSB_MASK) | valueToStore;
	registerop(storeLocation,'w',valueToStore); // write the computed value


}

uint16_t fetchInstruction()
{
		uint16_t adressOfInstruction = registerop(PC,'r',0);
		uint16_t currentInstruction = memory[adressOfInstruction]; // load the current instruction as the instruction currently pointed to by PC
		uint16_t nextInstrAdress = adressOfInstruction + 1;
		registerop(PC,'w', nextInstrAdress); // increment the program counter
		
		return currentInstruction;
}

