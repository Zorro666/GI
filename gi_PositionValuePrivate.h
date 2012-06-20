#ifndef GI_POSITIONVALUEPRIVATE_HH
#define GI_POSITIONVALUEPRIVATE_HH

struct gi_PositionValue
{
	char m_positionName[GI_POSITIONNAME_MAX_SIZE];
	GI_POSITION m_position;
	GI_POSITION m_basePosition;
	union PositionValue_Value 
	{
		float f;
		size_t i;
	} m_value;
	GI_QST m_qst;
	GI_TYPE m_valueType;
};

#endif /* #ifndef GI_POSITIONVALUEPRIVATE_HH */
