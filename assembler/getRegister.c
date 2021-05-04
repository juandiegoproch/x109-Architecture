#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "definitions.h"

uint8_t getRegister(char* regMnemonic)
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
	else if (!strcmp(regMnemonic,"SP"))
	{
		return SP;
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
