#include <stdint.h>
#include <stdbool.h>

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

while you are in word, keep track on where you are in the word, copy to buffer

if you get out of the word reset the index of where you are in the word
*/
	//DEBUG PRINT STATEMENTS
	
	printf(instruction);

	int wordNumber = 0;
	int inWordIndex = 0;
	char currentChar;
	bool inWord = !isWhitespace(instruction[0]);
	bool inString = false;
	for(int i=0;(currentChar = instruction[i]) != ';' || currentChar != '\0' ;i++)
	{
		if (currentChar == ';' || currentChar == '\n') break; // Weird fix taht shouldnt be f***ng necesary, but for loop refuses to terminate and im tired
		
		if (currentChar == '"') 
		{
			inString = !inString; // set the state to true when " encountered, then, on next " set to false
			continue;
		}
		
		if (isWhitespace(currentChar) && !inString) // if (currentchar is white and we are not in string.
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
