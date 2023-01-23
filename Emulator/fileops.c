#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "declarations.h"
#define MEMORY_SIZE_BYTES sizeof(uint16_t)*(RAMSIZE+1)

// Load Memory with data:
void memload(char* memimage_s)
{
	printf("Loading binary file %s \n",memimage_s);
	FILE* memimage_F = fopen(memimage_s,"r");
	size_t words_read = 0;
	if (memimage_F == NULL)
	{
		printf("Error reading binary file %s \n",memimage_s);
		exit(0xBE);
	}
	if ( (words_read = fread(memory,sizeof(uint16_t),(RAMSIZE+1),memimage_F)) >= (RAMSIZE+1)) 
		printf("WARNING: Memory image file provided overflowed from memory. \n");
	else if (words_read < (RAMSIZE+1)) 
		printf("WARNING: Memory image file provided not long enough to fully initialize memory. \n");
	fclose(memimage_F);
}
// Dump RAM to file

void memdump()
{
	FILE* ramdump_F = fopen("memdump.memimg","w");
	if (ramdump_F == NULL)
	{
		printf("Error dumping memory.");
		exit(0xBE);
	}
	fwrite(memory,sizeof(uint16_t),(RAMSIZE+1),ramdump_F);
	fclose(ramdump_F);
}
// Register Handler
