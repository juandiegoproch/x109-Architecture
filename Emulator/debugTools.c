#include <stdio.h>
#include <stdlib.h>

#include "declarations.h"

void printRegisterValues()
{
	printf("Register Values: \n");
	printf("PC: %x (int %d)\n",    registers[PC],registers[PC]);
	printf("JR: %x (int %d)\n",    registers[JR],registers[JR]);
	printf("ML: %x (int %d)\n",    registers[ML],registers[ML]);
	printf("R2/SBP: %x (int %d)\n",registers[R2],registers[R2]);
	printf("R1: %x (int %d)\n",    registers[R1],registers[R1]);
	printf("R0: %x (int %d)\n",    registers[R0],registers[R0]);
	printf("SP: %x (int %d)\n",    registers[SP],registers[SP]);
	printf("ZR: %x (int %d)\n",    registers[ZR],registers[ZR]);
}
void securePrintRam(int location, int look_width)
{
	int lowerBound = location-look_width > 0? location-look_width:0;
	int upperBound = location+look_width > RAMSIZE? RAMSIZE:location+look_width;
	
	for (int i=lowerBound;i<=upperBound;i++)
	{
		printf("0x%04X   0x%04X \n",i,memory[i]);
	}
}

void securePrintRamDissasembly(int location, int look_width)
{
	securePrintRam(location,look_width);
}

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

char* dissasembly(uint16_t assembled_instruction)
{
	bool first_bit = assembled_instruction >> 15;
	if (first_bit)
	{
		// is Mload instruction
		uint8_t reg;
		// get registers and clear

		// get whois
	}
	else
	{
		// is ALUOP instruction
		uint8_t str;
		uint8_t op1;
		uint8_t op2;
		// get registers and clear

		// get whois
	}

}