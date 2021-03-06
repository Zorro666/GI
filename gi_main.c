#include <stdio.h>
#include <string.h>

#include "gi.h"

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
	/*gi_LoadFile("broken.json", 1);*/

	gi_LoadFile("data/london_monarchs.json", debug);
	gi_LoadFile("data/offence/proset.json", debug);
	gi_LoadFile("data/defence/3-4.json", 1);
	gi_LoadFile("data/defence/3-5.json", 1);

	gi_Compute();
	gi_Output();
	return -1;
}
