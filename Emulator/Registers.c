#include <stdint.h>
#include "declarations.h"

uint16_t registerop(int reg,char mode,uint16_t value)
{
	if (mode == 'r') // Read Operation handler
	{
		if (reg>MD) // All non special behaviour regs
		{
			return registers[reg];
		} 
		else if (reg == MD) // Memory Data reg handler
		{
			return memory[registers[ML]]; 
		}
		else if (reg == ZR) // Zero register handler
		{
			return (uint16_t)0x0000;
		}
	}
	else if (mode == 'w')
	{
		if (reg == MD)
		{
			//MD storage handler
			memory[registers[ML]] = value; 
			registers[MD] = value; // update MD for consistency
			return value;
		}
		else
		{
		if (reg == ML) registers[MD] = memory[value]; // Update the MD data to be consistent
			registers[reg] = value;
			return value;
		}
	}
}

