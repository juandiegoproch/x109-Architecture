#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "declarations.h"

#define DUMP_MEMORY_FLAG "-d"
void parseCliArguments(int argc,char* argv[],char** fileToRead, bool* dumpMemoryOption)
{
	if (argc > 1)
	{
		*fileToRead = argv[1];
		if (argc > 2)
		{
			for (int i = 2;i < argc;i++)
				if (!strcmp(DUMP_MEMORY_FLAG,argv[i]))
					*dumpMemoryOption = true;
				else
				{
					printf("Invalid flag '%s'",argv[i]);
				}
		}
	}
	else
	{
		*dumpMemoryOption = false;
		fileToRead = NULL;
	}
	
}

