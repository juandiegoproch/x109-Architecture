#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "declarations.h"

#define DUMP_MEMORY_FLAG "-d"
#define DEBUGINFO_FLAG "-g"
void parseCliArguments(int argc,char* argv[],char** fileToRead, bool* dumpMemoryOption,bool* debuginfo)
{
	if (argc > 1)
	{
		*fileToRead = argv[1];
		if (argc > 2)
		{
			for (int i = 2;i < argc;i++)
				if (!strcmp(DUMP_MEMORY_FLAG,argv[i])) // FLAGS HANDLER
					*dumpMemoryOption = true;
				else if(!strcmp(DEBUGINFO_FLAG,argv[i]))
					*debuginfo = true;
				else
				{
					printf("Invalid flag '%s'",argv[i]);
					abort();
				}
		}
	}
	else
	{
		*dumpMemoryOption = false;
		fileToRead = NULL;
	}
	
}

