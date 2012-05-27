#include <memory.h>
#include <stdio.h>
#include "json.h"

/* true if character represent a digit */
#define IS_DIGIT(c) (c >= '0' && c <= '9')

/* convert string to integer */
char* atoi(const char* const first, const char* const last, int* const out)
{
	int result = 0;
	int sign = 1;
	char* start = (char*)first;
	if (start != last)
	{
		if (*start == '-')
		{
			sign = -1;
			++start;
		}
		else if (*start == '+')
		{
			++start;
		}
	}

	for (; start != last && IS_DIGIT(*start); ++start)
	{
		result = 10 * result + (*start - '0');
	}
	*out = result * sign;

	return start;
}

/* convert hexadecimal string to unsigned integer */
char* hatoui(const char* const first, const char* const last, unsigned int* const out)
{
	unsigned int result = 0;
	char* start = (char*)first;
	for (; start != last; ++start)
	{
		unsigned int digit;
		if (IS_DIGIT(*start))
		{
			digit = (unsigned int)(*start - '0');
		}
		else if (*start >= 'a' && *start <= 'f')
		{
			digit = (unsigned int)(*start - 'a' + 10);
		}
		else if (*start >= 'A' && *start <= 'F')
		{
			digit = (unsigned int)(*start - 'A' + 10);
		}
		else
		{
			break;
		}
		result = 16 * result + digit;
	}
	*out = result;

	return start;
}

/* convert string to floating point */
char* atof(const char* const first, const char* const last, float* const out)
{
	char* start = (char*)first;
	float sign = 1;
	float result = 0;
	int exponent_negative = 0;
	int exponent = 0;

	/* sign */
	if (start != last)
	{
		if (*start == '-')
		{
			sign = -1;
			++start;
		}
		else if (*start == '+')
		{
			++start;
		}
	}

	/* integer part */
	for (; start != last && IS_DIGIT(*start); ++start)
	{
		result = 10.0f * result + (float)(*start - '0');
	}

	/* fraction part */
	if (start != last && *start == '.')
	{
		float inv_base = 0.1f;

		++start;
		for (; start != last && IS_DIGIT(*start); ++start)
		{
			result += (float)(*start - '0') * inv_base;
			inv_base *= 0.1f;
		}
	}

	/* result w\o exponent */
	result *= sign;

	/* exponent */
	if (start != last && (*start == 'e' || *start == 'E'))
	{
		++start;

		if (*start == '-')
		{
			exponent_negative = 1;
			++start;
		}
		else if (*start == '+')
		{
			++start;
		}

		for (; start != last && IS_DIGIT(*start); ++start)
		{
			exponent = 10 * exponent + (*start - '0');
		}
	}

	if (exponent)
	{
		float power_of_ten = 10.0f;
		for (; exponent > 1; exponent--)
		{
			power_of_ten *= 10.0f;
		}

		if (exponent_negative)
		{
			result /= power_of_ten;
		}
		else
		{
			result *= power_of_ten;
		}
	}

	*out = result;

	return start;
}

json_value* json_alloc(block_allocator* allocator)
{
	json_value* value = (json_value*)block_allocator_malloc(allocator, sizeof(json_value));
	memset(value, 0, sizeof(json_value));
	return value;
}

void json_append(json_value* lhs, json_value* rhs)
{
	rhs->parent = lhs;
	if (lhs->last_child)
	{
		lhs->last_child = lhs->last_child->next_sibling = rhs;
	}
	else
	{
		lhs->first_child = lhs->last_child = rhs;
	}
}

#define ERROR(it, desc)\
{ \
	char* c; \
	*error_pos = it;\
	*error_desc = (char*)desc;\
	*error_line = 1 - escaped_newlines;\
	for (c = it; c != source; --c)\
	{ \
		if (*c == '\n') ++*error_line;\
	} \
	return 0; \
}

#define CHECK_TOP() if (!top) {ERROR(it, "Unexpected character");}

json_value* json_parse(char* const source, char* error_pos[], char* error_desc[], int* error_line, block_allocator* allocator)
{
	json_value* root = 0;
	json_value* top = 0;

	char* name = 0;
	char* it = source;

	int escaped_newlines = 0;

	while (*it)
	{
		switch (*it)
		{
			case '{':
			case '[':
				{
					/* create new value */
					json_value* object = json_alloc(allocator);

					/* name */
					object->name = name;
					name = 0;

					/* type */
					object->type = (*it == '{') ? JSON_OBJECT : JSON_ARRAY;

					/* skip open character */
					++it;

					/* set top and root */
					if (top)
					{
						json_append(top, object);
					}
					else if (!root)
					{
						root = object;
					}
					else
					{
						ERROR(it, "Second root. Only one root allowed");
					}
					top = object;
				}
				break;

			case '}':
			case ']':
				{
					if (!top || top->type != ((*it == '}') ? JSON_OBJECT : JSON_ARRAY))
					{
						ERROR(it, "Mismatch closing brace/bracket");
					}

					/* skip close character */
					++it;

					/* set top */
					top = top->parent;
				}
				break;

			case ':':
				if (!top || top->type != JSON_OBJECT)
				{
					ERROR(it, "Unexpected character");
				}
				++it;
				break;

			case ',':
				CHECK_TOP();
				++it;
				break;

			case '"':
				{
					char* first;
					char* last;
					CHECK_TOP();

					/* skip '"' character */
					++it;

					first = it;
					last = it;
					while (*it)
					{
						if ((unsigned char)*it < '\x20')
						{
							ERROR(first, "Control characters not allowed in strings");
						}
						else if (*it == '\\')
						{
							switch (it[1])
							{
								case '"':
									*last = '"';
									break;
								case '\\':
									*last = '\\';
									break;
								case '/':
									*last = '/';
									break;
								case 'b':
									*last = '\b';
									break;
								case 'f':
									*last = '\f';
									break;
								case 'n':
									*last = '\n';
									++escaped_newlines;
									break;
								case 'r':
									*last = '\r';
									break;
								case 't':
									*last = '\t';
									break;
								case 'u':
									{
										unsigned int codepoint;
										if (hatoui(it + 2, it + 6, &codepoint) != it + 6)
										{
											ERROR(it, "Bad unicode codepoint");
										}

										if (codepoint <= 0x7F)
										{
											*last = (char)codepoint;
										}
										else if (codepoint <= 0x7FF)
										{
											*last++ = (char)(0xC0 | (codepoint >> 6));
											*last = (char)(0x80 | (codepoint & 0x3F));
										}
										else if (codepoint <= 0xFFFF)
										{
											*last++ = (char)(0xE0 | (codepoint >> 12));
											*last++ = (char)(0x80 | ((codepoint >> 6) & 0x3F));
											*last = (char)(0x80 | (codepoint & 0x3F));
										}
									}
									it += 4;
									break;
								default:
									ERROR(first, "Unrecognized escape sequence");
							}

							++last;
							it += 2;
						}
						else if (*it == '"')
						{
							*last = 0;
							++it;
							break;
						}
						else
						{
							*last++ = *it++;
						}
					}

					if (!name && top->type == JSON_OBJECT)
					{
						/* field name in object */
						name = first;
					}
					else
					{
						/* new string value */
						json_value* object = json_alloc(allocator);

						object->name = name;
						name = 0;

						object->type = JSON_STRING;
						object->value_data.string_value = first;

						json_append(top, object);
					}
				}
				break;

			case 'n':
			case 't':
			case 'f':
				{
					json_value* object;
					CHECK_TOP();

					/* new null/bool value */
					object = json_alloc(allocator);

					object->name = name;
					name = 0;

					/* null */
					if (it[0] == 'n' && it[1] == 'u' && it[2] == 'l' && it[3] == 'l')
					{
						object->type = JSON_NULL;
						it += 4;
					}
					/* true */
					else if (it[0] == 't' && it[1] == 'r' && it[2] == 'u' && it[3] == 'e')
					{
						object->type = JSON_BOOL;
						object->value_data.int_value = 1;
						it += 4;
					}
					/* false */
					else if (it[0] == 'f' && it[1] == 'a' && it[2] == 'l' && it[3] == 's' && it[4] == 'e')
					{
						object->type = JSON_BOOL;
						object->value_data.int_value = 0;
						it += 5;
					}
					else
					{
						ERROR(it, "Unknown identifier");
					}

					json_append(top, object);
				}
				break;

			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					char* first;
					json_value* object;
					CHECK_TOP();

					/* new number value */
					object = json_alloc(allocator);

					object->name = name;
					name = 0;

					object->type = JSON_INT;

					first = it;
					while (*it != '\x20' && *it != '\x9' && *it != '\xD' && *it != '\xA' && *it != ',' && *it != ']' && *it != '}')
					{
						if (*it == '.' || *it == 'e' || *it == 'E')
						{
							object->type = JSON_FLOAT;
						}
						++it;
					}

					if (object->type == JSON_INT && atoi(first, it, &object->value_data.int_value) != it)
					{
						ERROR(first, "Bad integer number");
					}

					if (object->type == JSON_FLOAT && atof(first, it, &object->value_data.float_value) != it)
					{
						ERROR(first, "Bad float number");
					}

					json_append(top, object);
				}
				break;

			default:
				ERROR(it, "Unexpected character");
		}

		/* skip white space */
		while (*it == '\x20' || *it == '\x9' || *it == '\xD' || *it == '\xA')
		{
			++it;
		}
	}

	if (top)
	{
		ERROR(it, "Not all objects/arrays have been properly closed");
	}

	return root;
}

json_value* json_parse_file(char* const filename, char* error_pos[], char* error_desc[], int* error_line, block_allocator* allocator)
{
	char* buffer = NULL;
	FILE* fp = fopen(filename, "rb");
	size_t size;
	size_t numRead;

	if (fp == NULL)
	{
		fprintf(stderr, "ERROR opening '%s'\n", filename);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	size = (size_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = (char*)malloc(size);
	numRead = fread(buffer, 1, size, fp);
	fclose(fp);

	if (numRead != size)
	{
		fprintf(stderr, "ERROR reading from '%s' read %d bytes but expected %d bytes\n", filename, numRead, size);
		return NULL;
	}

	printf("Read %d bytes from '%s'\n", numRead, filename);
	return json_parse(buffer, error_pos, error_desc, error_line, allocator);
}
