#include <stdio.h>
#include <stdint.h>

#include "definitions.h"
int main(int argc,char* argv[])
{
	if (argc<3) printf("Error: Not enough input files \n");
	
	FILE* source;
	source = fopen(argv[1],"r");
	
	FILE* program;
	program = fopen(argv[2],"w");
	
	char* linebuffer = NULL;
	size_t linebufferlen = 0;
	
	char token1[MAXTOKENLEN] = {0};
	char token2[MAXTOKENLEN] = {0};
	char token3[MAXTOKENLEN] = {0};
	char token4[MAXTOKENLEN] = {0};
	char* tokenBuffer[4] = {token1,token2,token3,token4};
	
	while(getline(&linebuffer,&linebufferlen,source) != -1)
	{
		gettokens(tokenBuffer,linebuffer);
		uint16_t instruction = assmInstFromTokens(tokenBuffer);
		
		fwrite(&instruction,sizeof(uint16_t),1,program);
		// Zero out the individual tokens
		for (int i = 0; i<MAXTOKENLEN;i++)
		{
			token1[i] = '\0';
			token2[i] = '\0';
			token3[i] = '\0';
			token4[i] = '\0';
		}
	}
}
