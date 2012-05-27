#include <stdio.h>
#include "json.h"

#define IDENT(n) \
{ \
	int i; \
	for (i = 0; i < n; ++i) printf("    "); \
} 

void print(json_value *value, int ident)
{
	IDENT(ident);
	if (value->name) printf("\"%s\" = ", value->name);
	switch(value->type)
	{
		case JSON_NULL:
			printf("null\n");
			break;
		case JSON_OBJECT:
		case JSON_ARRAY:
		{
			json_value *it;
			printf(value->type == JSON_OBJECT ? "{\n" : "[\n");
			for (it = value->first_child; it; it = it->next_sibling)
			{
				print(it, ident + 1);
			}
			IDENT(ident);
			printf(value->type == JSON_OBJECT ? "}\n" : "]\n");
			break;
		}
		case JSON_STRING:
			printf("\"%s\"\n", value->value_data.string_value);
			break;
		case JSON_INT:
			printf("%d\n", value->value_data.int_value);
			break;
		case JSON_FLOAT:
			printf("%f\n", value->value_data.float_value);
			break;
		case JSON_BOOL:
			printf(value->value_data.int_value ? "true\n" : "false\n");
			break;
	}
}
int main(int argc, char* argv[])
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
	}

	{
		json_value* root;
		char* error_pos;
		char* error_desc;
		int error_line;
		block_allocator json_allocator;
		block_allocator_init(&json_allocator, 128*1024);

		root = json_parse_file("data/offense/proset.json", &error_pos, &error_desc, &error_line, &json_allocator);
		if (root)
		{
			print(root, 0);
		}
	}
	return -1;
}
