#include "gi.h"

Json_Value* FindOffencePlay(Json_Value* const value)
{
	return json_FindObjectByName(value, "Play");
}
