#ifndef GI_COMPARE_HH
#define GI_COMPARE_HH

#include <stdlib.h>

typedef struct SizetItem
{
	size_t m_value;
	size_t m_key;
} SizetItem;

extern int SizetItem_CompareSmaller(const void* a, const void* b);
extern int SizetItem_CompareBigger(const void* a, const void* b);

typedef struct FloatItem
{
	float m_value;
	size_t m_key;
} FloatItem;

extern int FloatItem_CompareSmaller(const void* a, const void* b);
extern int FloatItem_CompareBigger(const void* a, const void* b);

#endif /* #ifndef GI_COMPARE_HH */
