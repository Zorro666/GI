#include "gi_Compare.h"

int SizetItem_CompareSmaller(const void* a, const void* b)
{
	const size_t valueA = ((const SizetItem*)a)->m_value;
	const size_t valueB = ((const SizetItem*)b)->m_value;
	return (valueA < valueB);
}

int SizetItem_CompareBigger(const void* a, const void* b)
{
	const size_t valueA = ((const SizetItem*)a)->m_value;
	const size_t valueB = ((const SizetItem*)b)->m_value;
	return (valueA > valueB);
}

int FloatItem_CompareSmaller(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA < valueB);
}

int FloatItem_CompareBigger(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA > valueB);
}
