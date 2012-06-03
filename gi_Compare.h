#ifndef GI_COMPARE_HH
#define GI_COMPARE_HH

typedef struct IntItem
{
	int m_value;
	size_t m_key;
} IntItem;

static int intItem_Compare(const void* a, const void* b)
{
	const int valueA = ((const IntItem*)a)->m_value;
	const int valueB = ((const IntItem*)b)->m_value;
	return (valueA < valueB);
}

typedef struct FloatItem
{
	float m_value;
	size_t m_key;
} FloatItem;

static int floatItem_Compare(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA < valueB);
}

#endif /* #ifndef GI_COMPARE_HH */
