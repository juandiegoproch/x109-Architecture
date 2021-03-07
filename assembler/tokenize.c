#include <stdint.h>

#include "definitions.h"

int isWhitespace(char character)
{
	char whitespace[3] = {'\t',' ',','};
	for (int i = 0; i<3; i++)
	{
		if (character == whitespace[i]) return true; 
	}
	return 0;
}

int gettokens(char** buffer,char* instruction)
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
			*(*(buffer+wordNumber)+inWordIndex) = currentChar;
			inWordIndex++;
		}
	}
	return wordNumber;
}
