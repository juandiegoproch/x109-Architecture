#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "declarations.h"

#define DUMP_MEMORY_FLAG "-d"
#define DEBUGINFO_FLAG "-g"
void parseCliArguments(int argc,char* argv[],char** fileToRead, bool* dumpMemoryOption,bool* debuginfo)
{

	*dumpMemoryOption = false;
	*fileToRead = NULL;
	for(int i = 1; i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			//its a flag
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
		else
		{
			//is a file name
			if (*fileToRead == NULL)
				*fileToRead = argv[i];
			else
			{
				printf("Too many arguments %s",argv[i]);
				abort();
			}
		}
	}
	
}

