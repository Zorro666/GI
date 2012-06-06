#ifndef GI_LOGGERCHANNEL_HH
#define GI_LOGGERCHANNEL_HH

#include <stdarg.h>

#include "gi.h"

enum GI_LoggerChannelFlags 	{ GI_LOGGERCHANNEL_FLAGS_FATAL = (1<<0),
															GI_LOGGERCHANNEL_FLAGS_CONSOLE_OUTPUT = (1<<1),
															GI_LOGGERCHANNEL_FLAGS_FILE_OUTPUT = (1<<2)
														};

typedef struct gi_LoggerChannel gi_LoggerChannel;

struct gi_LoggerChannel
{
	const char* m_nickName;
	const char* m_prefixStr;
	const char* m_outputFileName;
	unsigned int m_flags;
};

void gi_LoggerChannel_Init(gi_LoggerChannel* const pThis, const char* const nickName, 
											const char* const prefix, const char* const outputFileName, const size_t flags);
void gi_LoggerChannel_Shutdown(gi_LoggerChannel* const pThis);

void gi_LoggerChannel_Error(gi_LoggerChannel* const pThis, const char* const fmt, ...);
void gi_LoggerChannel_Print(gi_LoggerChannel* const pThis, const char* const fmt, ...);

void gi_LoggerChannel_SetFlags(gi_LoggerChannel* const pThis, const size_t flags);
void gi_LoggerChannel_ChangeFlags(gi_LoggerChannel* const pThis, const size_t flags, const GI_Bool enable);
size_t gi_LoggerChannel_GetFlags(gi_LoggerChannel* const pThis);

void gi_LoggerChannel_SetOutputFileName(gi_LoggerChannel* const pThis, const char* const fname);
void gi_LoggerChannel_InternalOutput(gi_LoggerChannel* const pThis, const char* const fmt, va_list* pArgPtr);

#endif /*ifndef GI_LOGGERCHANNEL_HH*/
