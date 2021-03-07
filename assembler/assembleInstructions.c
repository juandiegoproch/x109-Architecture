#include <stdlib.h>
#include <stdint.h>

#include "definitions.h"


/* ========== 3 ARGUMENT INSTRUCTIONS =========== */
// ADD instruction
uint16_t assmAdd(char* arguments[3]) // Takes 3 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[2]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (ADD_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// SUB instruction
uint16_t assmSub(char* arguments[3]) // Takes 3 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[2]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (SUB_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// Negate and SuM instruction 
uint16_t assmNsm(char* arguments[3]) // Takes 3 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[2]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (NSM_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// LOR instruction
uint16_t assmLor(char* arguments[3]) // Takes 3 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[2]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (LOR_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}


// ADI instruction
uint16_t assmAdi(char* arguments[3]) // Takes 3 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[2]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (ADI_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}




/*   =========  TWO OPERAND INSTRUCTIONS ============ */

// INCrement instruction
uint16_t assmInc(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      ZR << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (INC_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// DECrement instruction
uint16_t assmDec(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      ZR << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (DEC_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// NOT instruction
uint16_t assmNot(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[1]) << OP1_OFFSET;
	uint16_t operand2 =      ZR << OP2_OFFSET;
	
	uint16_t assembledInstruction = (NOT_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// MOV instruction
uint16_t assmMov(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint16_t operand1 =      ZR << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (MOV_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

// BNE instruction
uint16_t assmBne(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = ZR << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[0]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (BNE_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}
// BEQ instruction
uint16_t assmBeq(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = ZR << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[0]) << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (BEQ_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}
// BLE instruction
uint16_t assmBle(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = ZR << STL_OFFSET;
	uint16_t operand1 =      getRegister(arguments[0])  << OP1_OFFSET;
	uint16_t operand2 =      getRegister(arguments[1]) << OP2_OFFSET;
	
	uint16_t assembledInstruction = (BLE_MASK | storeLocation | operand1 | operand2);
	
	return assembledInstruction;
}

/* ============== LOAD INSTRUCTIONS ================ */

uint16_t assmLdl(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint8_t literalValue =   (uint8_t)strtol(arguments[1],NULL,0);
	
	uint16_t assembledInstruction = (LDL_MASK | storeLocation | literalValue);
	
	return assembledInstruction;
}

uint16_t assmLdm(char* arguments[2]) // Takes 2 arguments
{
	// Fetch the required arguments and offset them to the required positions
	uint16_t storeLocation = getRegister(arguments[0]) << STL_OFFSET;
	uint8_t literalValue =   (uint8_t)strtoul(arguments[1],NULL,0);
	
	uint16_t assembledInstruction = (LDM_MASK | storeLocation | literalValue);
	
	return assembledInstruction;
}
