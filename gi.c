#include "gi.h"

Json_Value* FindOffencePlay(Json_Value* const value)
{
	return Json_FindObjectByName(value, "Play");
}
