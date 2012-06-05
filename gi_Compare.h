#ifndef GI_COMPARE_HH
#define GI_COMPARE_HH

typedef struct SizetItem
{
	size_t m_value;
	size_t m_key;
} SizetItem;

static int SizetItem_Compare(const void* a, const void* b)
{
	const size_t valueA = ((const SizetItem*)a)->m_value;
	const size_t valueB = ((const SizetItem*)b)->m_value;
	return (valueA < valueB);
}

typedef struct FloatItem
{
	float m_value;
	size_t m_key;
} FloatItem;

static int FloatItem_Compare(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA < valueB);
}

#endif /* #ifndef GI_COMPARE_HH */
