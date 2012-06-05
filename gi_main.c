#include <stdio.h>
#include <string.h>

#include "gi.h"

void sams(void)
{
	int a;
	int b;
	a =123456789;
	b =6;
	printf("result %d x %d = %d\n", a,b,a*b);
}

int main(int argc, char* argv[])
{
	size_t debug = 0;
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
		if (strcmp(argv[i], "-debug") == 0)
		{
			debug = 1;
		}
	}

	gi_Init();

	gi_LoadFile("data/offense/proset.json", debug);
	gi_LoadFile("data/london_monarchs.json", debug);

	gi_Output();
	sams();
	return -1;
}
