#include <stdio.h>

#include "declarations.h"

void printRegisterValues()
{
	printf("Register Values: \n");
	printf("PC: %x \n",    registers[PC]);
	printf("JR: %x \n",    registers[JR]);
	printf("ML: %x \n",    registers[ML]);
	printf("R2/SBP: %x \n",registers[R2]);
	printf("R1: %x \n",    registers[R1]);
	printf("R0: %x \n",    registers[R0]);
	printf("SP: %x \n",    registers[SP]);
	printf("ZR: %x \n",    registers[ZR]);
}
void securePrintRam(int location, int look_width)
{
	int lowerBound = location-look_width > 0? location-look_width:0;
	int upperBound = location+look_width > RAMSIZE? RAMSIZE:location+look_width;
	
	for (uint16_t i=lowerBound;i<=upperBound;i++)
	{
		printf("0x%04X   0x%04X \n",i,memory[i]);
	}
}