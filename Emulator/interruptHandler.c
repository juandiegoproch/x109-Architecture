#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "declarations.h"

int userInterruptHandler(int signum)
{
	
	printf("\nUser requested termination of program execution. \n");
	memdump();
	printf("Memory Dumped to file 'memdump.memimg'. \n");
	printf("Register Values at Interrupt: \n");
	printf("PC: %x \n",registers[PC]);
	printf("JR: %x \n",registers[JR]);
	printf("ML: %x \n",registers[ML]);
	printf("R2: %x \n",registers[R2]);
	printf("R1: %x \n",registers[R1]);
	printf("R0: %x \n",registers[R0]);
	printf("MD: %x \n",registers[MD]);
	printf("ZR: %x \n",registers[ZR]);
	
	exit(0xEE);
}
