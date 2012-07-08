#ifndef JSON_PRIVATE_H
#define JSON_PRIVATE_H

struct Json_Value
{
	Json_Value* m_parent;
	Json_Value* m_next_sibling;
	Json_Value* m_first_child;
	Json_Value* m_last_child;

	char* m_name;
	union Value
	{
		char* string_value;
		int int_value;
		float float_value;
	} m_value_data;

	Json_Type m_type;
};

#endif
