#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#include "declarations.h"

#define ALUOP 1
#define MLOAD 0

void debugConsole()
		{
			printf("\n Execution Halted \n");
			char command;
			while ((command = getchar()) != 'n')
			{
				switch (command)
				{
					case 'r':
					{
						printRegisterValues();
						break;
					}
					case 'm':
					{
						securePrintRam(registerop(ML,'r',0),5);
						break;
					}
					case 's':
					{
						securePrintRam(registerop(SP,'r',0),5);
						break;
					}
					case 'i':
					{
						securePrintRam(registerop(PC,'r',0),5);
						break;
					}
					default:
						printf("Debug command not recognised \n");
				}
				fflush(stdin);
			}
			fflush(stdin);

		}

int main(int argc,char* argv[])
{
	// Bind Signal handler
	
	signal(SIGINT,(void*)userInterruptHandler);
	
	// Get CLI arguents
	char* fileToRead = NULL;
	bool dumpRam = false;
	bool debugInfo = false;
	
	parseCliArguments(argc,argv,&fileToRead, &dumpRam, &debugInfo);
	
	if (fileToRead != NULL) memload(fileToRead); // Load memimg file into ram
	
	// Main Execution Loop
	while(!memory[RAMSIZE])
	{
		if (debugInfo) // DEBUG HANDLER
			debugConsole();
		
		// Terminal
		if (memory[33]) // 0x21 tells terminal to output. 
		{
			putchar(memory[34]); // 0x22 holds char to print
			memory[33] = 0; // Reset 0x21
		}


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
	if (debugInfo)
	{
			printf("\n Execution Terminated  with code hex(%x), char(%c)\n",memory[RAMSIZE],memory[RAMSIZE]);
			printRegisterValues();
			getchar();
	}

	return 0;
}
