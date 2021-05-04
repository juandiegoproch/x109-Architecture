#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "definitions.h"

uint16_t memory[MAXRAM] = {(uint16_t)0};

int main(int argc,char* argv[])
{
	if (argc<3) printf("Error: Not enough input files \n");
	
	FILE* source;
	source = fopen(argv[1],"r");
	
	FILE* program;
	program = fopen(argv[2],"w");
	
	char* linebuffer = NULL;
	size_t linebufferlen = 0;
	
	char *token1 = calloc(MAXTOKENLEN,sizeof(char));
	char *token2 = calloc(MAXTOKENLEN,sizeof(char));
	char *token3 = calloc(MAXTOKENLEN,sizeof(char));
	char *token4 = calloc(MAXTOKENLEN,sizeof(char));
	char *tokenBuffer[4] = {token1,token2,token3,token4};
	
	int currentLine = 0;
	int currentAddress = 0;
	while(getline(&linebuffer,&linebufferlen,source) != -1)
	{
		gettokens(tokenBuffer,linebuffer);
		if (tokenBuffer[0][0] == '\n' || tokenBuffer[0][0] == '\0')
			continue; // Ignore line if empty

		// HANDLERS FOR ASSEMBLER DIRECTIVES
		if (!strcmp(tokenBuffer[0],".address"))
		{
			// Handle ADRESS symbol (Wich sets the next line's adress to that of it's argument.
			// .ADRESS nextInstructionAdress
			currentAddress = (uint16_t)strtoul(tokenBuffer[1],NULL,0);
		}
		else if (!strcmp(tokenBuffer[0],".litval"))
		{
			// .LITVAL badress, val (opt)ammount;
			uint16_t base_address = (uint16_t)strtoul(tokenBuffer[1],NULL,0);
			uint16_t value = (uint16_t)strtoul(tokenBuffer[2],NULL,0);
			int ammount;
			
			if (tokenBuffer[3][0] == '\0') // Test if string empty
				ammount = 1;
			else 
				ammount = (int)strtoul(tokenBuffer[3],NULL,0);
			
			for (int i = 0; i < ammount; i++)
				memory[base_address+i] = value;
		}
		else if(!strcmp(tokenBuffer[0],".stringlit"))
		{
			// string literal handling
			uint16_t base_address = (uint16_t)strtoul(tokenBuffer[1],NULL,0);
			
			uint16_t charToCp;
			for (int i=0;(charToCp = (uint16_t)tokenBuffer[2][i]) != (uint16_t)'\0';i++)
			{
				memory[base_address+i] = charToCp;
			}

		}
		else
		{
			uint16_t instruction = assmInstFromTokens(tokenBuffer,currentLine);
			memory[currentAddress] = instruction;
			currentAddress++;
		}
		// Zero out the individual tokens
		for (int i = 0; i<MAXTOKENLEN;i++)
		{
			token1[i] = '\0';
			token2[i] = '\0';
			token3[i] = '\0';
			token4[i] = '\0';
		}
		
		currentLine++;
		if (currentAddress >= MAXRAM-1) // -1 for array adressing shenanigans
		{
			printf("Memory Image defines data outside ram bounds at line %d",currentLine);
			abort();
		}
	}
	
	// Free Token arrays



	fwrite(memory,sizeof(uint16_t),MAXRAM,program);
}
