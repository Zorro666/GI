#include <stdio.h>

#include "gi.h"
#include "OffencePlay.h"

int main(int argc, char* argv[])
{
	Json_Value* root;
	int debug = 1;
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
	}

	gi_Init();

	root = gi_ParseFile("data/offense/proset.json", debug);
	if (root)
	{
		OffencePlay play;
		Json_Value* playRoot;
		playRoot = gi_FindOffencePlay(root);
		OffencePlay_Load(&play, playRoot);
		OffencePlay_Print(&play);
	}
	return -1;
}
