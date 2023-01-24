#include <stdio.h>

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