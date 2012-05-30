#include <stdio.h>

#include "gi.h"

int main(int argc, char* argv[])
{
	int debug = 1;
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
	}

	gi_Init();

	gi_LoadFile("data/offense/proset.json", debug);
	gi_LoadFile("data/london_monarchs.json", debug);

	return -1;
}
