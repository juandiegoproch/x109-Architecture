#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define true 1
#define false 0

#define MAXWORDS 5
#define MAXWORDLEN 5
#define MAXLINELEN 255

#define ADD_MASK 	0b1000000000000000
#define SUB_MASK   	0b1000001100000000
#define NSM_MASK	0b1000001000000000
#define MOV_MASK	0b1000111100000000
#define LOR_MASK	0b1000100000000000
#define NOT_MASK 	0b1000001000000000
#define ADI_MASK	0b1000000100000000
#define INC_MASK	0b1000010100000000
#define DEC_MASK	0b1000011000000000
#define LDL_MASK	0b0000000000000000
#define LDM_MASK	0b0000010000000000
#define BNE_MASK	0b1000001100000010
#define BEQ_MASK	0b1000001100000001
#define BLE_MASK	0b1000001100000011
#define PC (uint16_t)7
#define JR (uint16_t)6
#define ML (uint16_t)5
#define R2 (uint16_t)4
#define R1 (uint16_t)3
#define R0 (uint16_t)2
#define MD (uint16_t)1 //- Special Behaviour
#define ZR (uint16_t)0 //- Special Behaviour

int getinstfromfile(FILE* file,char buffer[])
{
	int currentchar = 0;
	for(int i = 0; (currentchar = getc(file)) != '\n';i++)
	{
		if (currentchar == EOF) return -1;
		
		buffer[i] = currentchar;
		
	}
	return 0;
} 

uint8_t translateRegister(char* regMnemonic)
{
	if (!strcmp(regMnemonic,"PC"))
	{
		return PC;
	} 
	else if (!strcmp(regMnemonic,"JR"))
	{
		return JR;
	}
	else if (!strcmp(regMnemonic,"ML"))
	{
		return ML;
	}
	else if (!strcmp(regMnemonic,"R2"))
	{
		return R2;
	}
	else if (!strcmp(regMnemonic,"R1"))
	{
		return R1;
	}
	else if (!strcmp(regMnemonic,"R0"))
	{
		return R0;
	}
	else if (!strcmp(regMnemonic,"MD"))
	{
		return MD;
	}
	else if (!strcmp(regMnemonic,"ZR"))
	{
		return ZR;
	}
	else
	{
		printf("Invalid Register Mnemonic \" %s \" \n",regMnemonic);
		return 0;
	}

}
uint16_t translateInstruction(char* instMnemonic)
{
	if (!strcmp(instMnemonic,"ADD")) //1
	{
		return ADD_MASK;
	} 
	else if (!strcmp(instMnemonic,"SUB")) // 2
	{
		return SUB_MASK;
	}
	else if (!strcmp(instMnemonic,"NSM")) // 3
	{
		return NSM_MASK;
	}
	else if (!strcmp(instMnemonic,"MOV")) // 4
	{
		return MOV_MASK;
	}
	else if (!strcmp(instMnemonic,"LOR")) // 5
	{
		return LOR_MASK;
	}
	else if (!strcmp(instMnemonic,"NOT")) // 6
	{
		return NOT_MASK;
	}
	else if (!strcmp(instMnemonic,"ADI")) //7
	{
		return ADI_MASK;
	}
	else if (!strcmp(instMnemonic,"INC")) //8
	{
		return INC_MASK;
	}
	else if (!strcmp(instMnemonic,"DEC")) //9
	{
		return DEC_MASK;
	}
	else if (!strcmp(instMnemonic,"LDL")) //10
	{
		return LDL_MASK;
	}
	else if (!strcmp(instMnemonic,"LDM")) //11
	{
		return LDM_MASK;
	}
	else if (!strcmp(instMnemonic,"BNE")) //12
	{
		return BNE_MASK;
	}
	else if (!strcmp(instMnemonic,"BEQ")) //13
	{
		return BEQ_MASK;
	}
	else if (!strcmp(instMnemonic,"BLE")) //14
	{
		return BLE_MASK;
	}
	else
	{
		printf("Invalid Instruction Mnemonic \" %s \" \n",instMnemonic);
		return 0x0000;
	}
}

// Tokenizing? (Word Detection)

int isWhitespace(char character)
{
	char whitespace[3] = {'\t',' ',','};
	for (int i = 0; i<3; i++)
	{
		if (character == whitespace[i]) return true; 
	}
	return 0;
}

int gettokens(char buffer[MAXWORDS][MAXWORDLEN],char* instruction)
{
/*
Assume a buffer that can hold each word that exists in the inst

detect if in word by detecting if you are in a whitespace or a non whitespace

while you are in word, keep track on where you arein the word, copy to buffer

if you get out of the word reset the index of where you are in the word
*/
	int wordNumber = 0;
	int inWordIndex = 0;
	char currentChar = instruction[0];
	int inWord = !isWhitespace(instruction[0]);
	for(int i=0;!(*(instruction+i) == ';' || *(instruction+i) == '\0') ;i++)
	{
	currentChar = instruction[i];
	if (isWhitespace(currentChar))
		{
			if (inWord) wordNumber++;
			inWord = false;
			inWordIndex = 0;
		}
		else
		{
			inWord = true;
			buffer[wordNumber][inWordIndex] = currentChar;
			inWordIndex++;
		}
	}
	return wordNumber;
}

uint16_t translatetokenizedinstruction(char tokens[MAXWORDS][MAXWORDLEN])
{
	uint16_t instructionBitMask = translateInstruction(tokens[0]);
	
	int instructionType = (instructionBitMask >> 15); // true is aluop, false is ld
	 
	uint16_t argument1 = translateRegister(tokens[1]) << 12;
	
	uint16_t argument3 = instructionType? (translateRegister(tokens[3]) << 2):0x0;
	
	uint16_t argument2 = instructionType? (translateRegister(tokens[2]) << 5):(uint16_t)strtol(tokens[2], NULL,0);
	
	return (instructionBitMask | argument1 | argument2 | argument3);
	
	
}

uint16_t assembleinstruction(char* instruction_s)
{
	char tokens[MAXWORDS][MAXWORDLEN] = {{'\0'}};
	gettokens(tokens,instruction_s);
	uint16_t instruction_r = translatetokenizedinstruction(tokens);
	return instruction_r;
}


int main(int argc,char* argv[])
{
// PARSE ARGUMENTS, OPEN FILES
	FILE* programToAssemble_F;
	FILE* binary_F;
	
	if (argc >= 1)
		programToAssemble_F = fopen(argv[1],"r");
	else
	{
		printf("No input files \n");
		return 1;
	}
	if (argc >= 2)
		binary_F = fopen(argv[2],"w");
	else
	{
		binary_F = fopen("a.memimg","w");
	}
	
// ASSEMBLE
	
	char lineBuffer[MAXLINELEN] = {'\0'};
	while (getinstfromfile(programToAssemble_F,lineBuffer) != -1) // If EOF Found
	{
		uint16_t instruction = assembleinstruction(lineBuffer); // Linebuffer set at the while loop
		fwrite(&instruction,sizeof(uint16_t),1,binary_F);
	}
	fclose(programToAssemble_F);
	fclose(binary_F);
}
