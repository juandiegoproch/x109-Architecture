#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"

uint16_t assmInstFromTokens(char** tokens,int line)
{
	uint16_t instructionToReturn = 0;
	if (!strcmp(tokens[0],"") || !strcmp(tokens[0],"NOP"))
	{
		instructionToReturn = assmNop(tokens+1);
	}
	else if (!strcmp(tokens[0],"ADD")) //ADD INSTRUCTION
	{
		instructionToReturn = assmAdd(tokens+1); //Pass pointer to 
	} 
	else if (!strcmp(tokens[0],"SUB")) // 2
	{
		instructionToReturn = assmSub(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"NSM")) // 3
	{
		instructionToReturn = assmNsm(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"MOV")) // 4
	{
		instructionToReturn = assmMov(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"LOR")) // 5
	{
		instructionToReturn = assmLor(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"NOT")) // 6
	{
		instructionToReturn = assmNot(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"ADI")) //7
	{
		instructionToReturn = assmAdi(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"INC")) //8
	{
		instructionToReturn = assmInc(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"DEC")) //9
	{
		instructionToReturn = assmDec(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"LDL")) //10
	{
		instructionToReturn = assmLdl(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"LDM")) //11
	{
		instructionToReturn = assmLdm(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"LD"))
	{
		instructionToReturn = assmLd(tokens+1);
	}
	else if (!strcmp(tokens[0],"ST"))
	{
		instructionToReturn = assmSt(tokens+1);
	}
	else if (!strcmp(tokens[0],"PUSH"))
	{
		instructionToReturn = assmPush(tokens+1);
	}
	else if (!strcmp(tokens[0],"POP"))
	{
		instructionToReturn = assmPop(tokens+1);
	}
	else if (!strcmp(tokens[0],"BNE")) //12
	{
		return instructionToReturn = assmBne(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"BEQ")) //13
	{
		instructionToReturn = assmBeq(tokens+1); //Pass pointer to 
	}
	else if (!strcmp(tokens[0],"BLE")) //14
	{
		instructionToReturn = assmBle(tokens+1); //Pass pointer to 
	}
	else
	{
		// Instruction not recognised handler
		printf("Instruction '%s' is not recognized in line %d.\n",tokens[0],line+1);
		printf("Assembly couldn't complete due to errors.");
		abort(); // Program has encountered irecoverable error and must exit.
		return 0x0000;
	}
	return instructionToReturn;
}
