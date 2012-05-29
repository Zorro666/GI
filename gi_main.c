#include <stdio.h>

#include "gi.h"
#include "OffencePlay.h"

int main(int argc, char* argv[])
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
	}

	{
		Json_Value* root;
		char* error_pos;
		char* error_desc;
		int error_line;
		BlockAllocator json_allocator;
		BlockAllocator_Init(&json_allocator, 128*1024);

		root = Json_ParseFile("data/offense/proset.json", &error_pos, &error_desc, &error_line, &json_allocator);
		if (root)
		{
			Json_Print(root, 0);
		}
		if (root)
		{
			OffencePlay play;
			Json_Value* playRoot;
			playRoot = FindOffencePlay(root);
			OffencePlay_Load(&play, playRoot);
		}
	}
	return -1;
}
