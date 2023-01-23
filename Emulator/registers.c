#include <stdint.h>
#include "declarations.h"

uint16_t registerop(int reg,char mode,uint16_t value)
{
	switch (mode)
	{
	case 'r':
		if (reg == ZR) // Zero register handler
		{
			return (uint16_t)0x0000;
		}

		return registers[reg];
		break;
	case 'w':
		registers[reg] = value;
		return value;
		break;
	
	default:
		printf("Invalid register access mode");
		abort();
		return (uint16_t) 0;
		break;
	}
}

