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
