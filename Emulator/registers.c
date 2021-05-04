#include <stdint.h>
#include "declarations.h"

uint16_t registerop(int reg,char mode,uint16_t value)
{
	if (mode == 'r') // Read Operation handler
	{
		if (reg == ZR) // Zero register handler
		{
			return (uint16_t)0x0000;
		}
		return registers[reg];
	}
	else if (mode == 'w')
	{
		registers[reg] = value;
		return value;
	}
}

