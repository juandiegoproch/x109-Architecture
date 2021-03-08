#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "declarations.h"

#define ALUOP 1
#define MLOAD 0

int main(int argc,char* argv[])
{
	// Get CLI arguents
	char* fileToRead = NULL;
	bool dumpRam = false;
	
	parseCliArguments(argc,argv,&fileToRead, &dumpRam);
	
	if (fileToRead != NULL) memload(fileToRead); // Load memimg file into ram
	
	// Main Execution Loop
	while(memory[RAMSIZE] == 0)
	{
		uint16_t currentInstruction = fetchInstruction();
		if (decode(currentInstruction) == ALUOP)
		{
			executeAluOpInstruction(currentInstruction);
		}
		else
		{
			executeMLoadInstruction(currentInstruction);
		}
	}
	
	printf("Program halted with code %x \n",memory[RAMSIZE]);
	
	// RAM dumping
	if (dumpRam)
		memdump();
		
	
	
	return 0;
}
