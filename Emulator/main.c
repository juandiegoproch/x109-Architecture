#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#define MEMCOMPILE

#include "posix.h"
#include "declarations.h"

#define ALUOP 1
#define MLOAD 0

void debugConsole()
{
	char command;
	static char lastcommand = ' ';
	static uint16_t break_on_pc = 0;
	static bool proceed = false;

	if (registers[PC] == break_on_pc)
	{
		proceed = false; //arrived at entry point
	}

	if (proceed)
		return; // skip all

	printf("\n Execution Halted \n");

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
				securePrintRamDissasembly(registerop(PC,'r',0),5);
				break;
			}
			case 'n':
			{
				; // pass
			}
			case 'c':
			{
				char number[10];
				fgets(number, 10, stdin);
				break_on_pc = strtol(number,NULL,10);
				proceed = true;
				fflush(stdin);
				return;
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
	
	debugInfo = true;

	if (fileToRead != NULL) memload(fileToRead); // Load memimg file into ram
	
	#ifdef MEMCOMPILE
		// initialize memory at compilation for debuggin purposes
		// R0 010
		// R1 011
		// R2 100
		// ML 101
		// SP 001

		//._start
		memory[0]  = 0b0010000000000010; // LLS R0 2
		memory[1]  = 0b0010010000000000; // LMS R0 0
		memory[2]  = 0b0011000000000001; // LLS R1 1
		memory[3]  = 0b0011010000000000; // LMS R1 0
		memory[4]  = 0b1100000001101000; // ADD R2 R1 R0, 2+1
		memory[5]  = 0b1100001101101000; // SUB R2 R1 R0, 2-1
		memory[6]  = 0b1100100001101000; // LOR R2 R1 R0, 2 or 1: 3
		memory[7]  = 0b1100000110000000; // INC R2: 4
		memory[8]  = 0b1100011000010000; // DEC R2: 3
		memory[9]  = 0b1100001000000000; // NOT R2 R0: FF
		memory[10] = 0b1100010000000000; // MOV R2 R0: 0
		memory[11] = 0b1001001000000000; // NOT SP R0: FF // initialize stack!
		memory[12] = 0b0011101000000001; // PUSH R1 1: PUSH 2 (use s to confirm)
		memory[13] = 0b0011001000000001; // POP R1 1: POP 3 
		memory[14] = 0b0101000001000000; // LLS ML 64
		memory[15] = 0b0001100100000100; // STH SP 4
		memory[16] = 0b0010000100000100; // LDH R0 4
		memory[17] = 0b0001100111111100; // STH SP -4
		memory[18] = 0b0100000111111100; // LDH R2 -4
		memory[19] = 0b0001000001000000; // LLS SP 64
		memory[20] = 0b0001010000000000; // LMS SP 0
		memory[21] = 0b0001101100000100; // STST SP 4
		memory[22] = 0b0010001100000100; // LDST R0 4
		memory[23] = 0b0001101111111100; // STST SP -4
		memory[24] = 0b0010001111111100; // LDST R0 -4
		memory[25] = 0b0000101000000000; // PUSH ZR;

		// .loop_while_ml_geq_r0: (26)
		memory[26] = 0b0010001000000000; // POP R0
		memory[27] = 0b1010000101000000; // INC R0
		memory[28] = 0b0010101000000000; // PUSH R0
		memory[29] = 0b0110000000011010; // SET JR .loop_while_ml_geq_r0
		memory[30] = 0b1000001110101010; // BNE ML R0; R0 - ML

		// ._exit_with_code_r0 (30)
		memory[31] = 0b1101001000000000; // NOT ML ZR, ML = FFFF
		memory[32] = 0b0010100100000000; // STH R0 0 // exit code on R0


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
