#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void help()
{
	printf("Usage: nmatch PATTERN STRING\n");
	printf("nmatch: a nucleotide sequence naive matching algorithm\n");
	printf("Returns PATTERN's start indices in STRING\n");
	printf("Preprocesing time: 0, matching time: O((n-m+1) m)\n");
	printf("Note: soft-masked sequences are treated equally as standard\n");
	printf("Example: nmatch UGAWACGT tsaaacgttgatacgt\n0 8\n\n");

}

int consensus(const char* s1, const char* s2)
{
	// compare and break as soon as at least
	// one match is found
	for(int j=0; j <strlen(s1); ++j)
	{
		for(int i=0; i<strlen(s2); ++i)
		{
			if(s1[j]==s2[i])
			{
				return 1;
				break;
			}
		}
	}
	return 0;
}

const char* hash(char c)
{
	//convert to upper and hash symbols
	switch(toupper(c))
	{
		case('A'):
		return "A";
		case('C'):
		return "C";
		case('G'):
		return "G";
		case('T'):
		return "TU";
		case('U'):
		return "UT";
		case('M'):
		return "AC";
		case('R'):
		return "AG";
		case('W'):
		return "ATU";
		case('S'):
		return "CG";
		case('Y'):
		return "CTU";
		case('K'):
		return "GTU";
		case('V'):
		return "ACG";
		case('H'):
		return "ACTU";
		case('D'):
		return "AGTU";
		case('B'):
		return "CGTU";
		case('N'):
		return "ACGTU";
		case('-'):
		return "-";
		default:
		return "-";
	}
}

int* naiveMatch(char* pattern, char* string)
{
	/*	R: A or G
		N: any
		W: A or T
		Y: C or T
		B: T or G or C
		Ð: A or T or G	*/
		
	int slen = strlen(string);
	int plen = strlen(pattern);

	// start by allocating some memory
	int* p;
	int allocated_mem = 4096;
	p = (int *) malloc(sizeof(int) * allocated_mem);

	// make sure memory was allocated successfully
	if(p==NULL)
	{
		fprintf(stderr, "Could not allocate memory. Aborting.\n");
		exit(1);		
	}

	// m as in memory management index
	unsigned int m = 0;
	for(int i=0; i < slen - plen + 1; ++i)
	{
		int match = 1;
		for (int j=0; j < plen; ++j)
		{
			//is O((n-m+1) m)
			if(!consensus(hash(pattern[j]),hash(string[j+i])) )
			{
				match = 0;
				break;
			}
		}
		if(match)
		{	
			//check where we are at with array size
			if (m < allocated_mem)
			{
				p[m] = i;
				++m;				
			}
			else
			{	
				// if about to exceed, allocate double the memory
				allocated_mem += 4096;
				p = realloc(p, sizeof(int) * allocated_mem);
				if(p==NULL)
				{
					fprintf(stderr, "Could not allocate memory. Aborting.\n");
					exit(1);		
				}
				p[m] = i;
				++m;
			}
		}
	}
	
	// allocate sizeof(int) extra bytes for one last digit
	p = realloc(p, sizeof(int) * allocated_mem + 1);
	if(p==NULL)
	{
		fprintf(stderr, "Could not allocate memory. Aborting.\n");
		exit(1);		
	}

	//last element (-1) will inform main function to break when found
	p[m] = -1;

	//return but REMEMBER to free()
	return p;
}

int main(int argc, char* argv[])
{	
	// prompt user with help
	if (argc == 1 || argc > 3)
	{
		printf("Usage: nmatch PATTERN STRING\nTry 'nmatch --help' for more information\n");
		exit(1);
	}

	if (argc==2 && strcmp(argv[1],"--help")==0)
	{
		help();
		exit(1);
	}

	if (argc==2 && strcmp(argv[1],"--help")!=0)
	{
		printf("Usage: nmatch PATTERN STRING\nTry 'nmatch --help' for more information\n");
		exit(1);		
	}

	if(strlen(argv[2]) < strlen(argv[1]))
	{
		printf("Usage: nmatch PATTERN STRING\nTry 'nmatch --help' for more information\n");
		exit(1);		
	}

	// check that input is characters only

	for (int i=0;i<strlen(argv[1]);++i)
	{
		if(isalpha(argv[1][i])==0)
		{
		printf("Usage: nmatch PATTERN STRING\nTry 'nmatch --help' for more information\n");
		exit(1);			
		}
	}

	for (int i=0;i<strlen(argv[2]);++i)
	{
		if(isalpha(argv[2][i])==0)
		{
		printf("Usage: nmatch PATTERN STRING\nTry 'nmatch --help' for more information\n");
		exit(1);			
		}
	}

	// declare a pointer-to-int to store matches indices
	int* match = naiveMatch(argv[1], argv[2]);

	// print match position start until -1 is found
	int i = 0;
	while(match[i]!=-1)
	{
		printf("%d ", match[i]);
		++i;
	}

	// print newline if there was at least a match
	if(i)
		printf("\n");

	// give memory back to the OS
	free(match);

	return 0;
}
