#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#define MEMCOMPILE

#include "posix.h"
#include "declarations.h"

#define ALUOP 1
#define MLOAD 0


int main(int argc,char* argv[])
{
	// Bind Signal handler
	
	signal(SIGINT,(void*)userInterruptHandler);
	
	// Get CLI arguents
	char* fileToRead = NULL;
	bool dumpRam = false;
	bool debugInfo = false;
	
	parseCliArguments(argc,argv,&fileToRead, &dumpRam, &debugInfo);
	
	debugInfo = true;

	if (fileToRead != NULL) memload(fileToRead); // Load memimg file into ram
	
	#ifdef MEMCOMPILE
		// initialize memory at compilation for debuggin purposes
		// R0 010
		// R1 011
		// R2 100
		// ML 101
		// SP 001

		memory[0]  = 0b0010000000000010; //
		memory[1]  = 0b0010010000000000; //
		memory[2]  = 0b0011000000000001; //
		memory[3]  = 0b0011010000000000; //
		memory[4]  = 0b1100000001101000; //
		memory[5]  = 0b1100001101101000; //
		memory[6]  = 0b1100100001101000; //
		memory[7]  = 0b1100000110000000; //
		memory[8]  = 0b1100011000010000; //
		memory[9]  = 0b1100001000000000; //
		memory[10] = 0b1100010000000000; //
		memory[11] = 0b1001001000000000; //
		memory[12] = 0b0011101000000001; //
		memory[13] = 0b0011001000000001; //
		memory[14] = 0b0101000001000000; //
		memory[15] = 0b0001100100000100; //
		memory[16] = 0b0010000100000100; //
		memory[17] = 0b0001100111111100; //
		memory[18] = 0b0100000111111100; //
		memory[19] = 0b0001000001000000; //
		memory[20] = 0b0001010000000000; //
		memory[21] = 0b0001101100000100; //
		memory[22] = 0b0010001100000100; //
		memory[23] = 0b0001101111111100; //
		memory[24] = 0b0010001111111100; //
		memory[25] = 0b0000101000000000; //
		                                 //
		memory[26] = 0b0010001000000000; //
		memory[27] = 0b1010000101000000; //
		memory[28] = 0b0010101000000000; //
		memory[29] = 0b0110000000011010; //
		memory[30] = 0b1000001110101010; //
		                                 //
		memory[31] = 0b1101001000000000; //
		memory[32] = 0b0010100100000000; //

		// hay que haced que las ALUOP sean ALUOP STR OP2 OP1 para que la semantica de las SUB sean mas sencillas
	#endif

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

		// set cpu speed (time it takes for instruction to run)
		Sleep(1);
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
