#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gi_LoggerChannel.h"
#include "gi_Logger.h"

/* ///////////////////////////////////////////////////////////////////////////////////////////////// */
/* // */
/* // Private functions */
/* // */
/* ///////////////////////////////////////////////////////////////////////////////////////////////// */

void gi_LoggerChannel_SetOutputFileName(gi_LoggerChannel* const pThis, const char* const fname)
{
	if (strcmp(pThis->m_outputFileName, fname) != 0)
	{
		FILE* filePtr;
		pThis->m_outputFileName = fname;
		filePtr = fopen(pThis->m_outputFileName, "wb");
		if (filePtr == NULL)
		{
			return;
		}
		fclose(filePtr);
	}
}

void gi_LoggerChannel_InternalOutput(gi_LoggerChannel* const pThis, const char* const fmt, va_list* pArgPtr)
{
	char outputBuffer[1024];
	va_list argPtr = *pArgPtr;
	const size_t flags = pThis->m_flags;
	const char* const prefix = pThis->m_prefixStr;

	vsnprintf(outputBuffer, sizeof(outputBuffer), fmt, argPtr);
	va_end(argPtr);

	if (flags & GI_LOGGERCHANNEL_FLAGS_CONSOLE_OUTPUT)
	{
		fprintf(stdout, "%s%s\n", prefix, outputBuffer);
	}
	if (flags & GI_LOGGERCHANNEL_FLAGS_FILE_OUTPUT)
	{
		const char* const fileName = pThis->m_outputFileName;
		FILE* const filePtr = fopen(fileName, "ab");
		if (filePtr)
		{
			fprintf(filePtr, "%s%s\n", prefix, outputBuffer);
			fflush(filePtr);
			fclose(filePtr);
		}
	}
	if (flags & GI_LOGGERCHANNEL_FLAGS_FATAL)
	{
		gi_Logger_SetErrorStatus();
		exit(-1);
	}
}

/* ///////////////////////////////////////////////////////////////////////////////////////////////// */
/* // */
/* // Public functions */
/* // */
/* ///////////////////////////////////////////////////////////////////////////////////////////////// */

void gi_LoggerChannel_Init(gi_LoggerChannel* const pThis, const char* const nickName, 
													 const char* const prefix, const char* const outputFileName, const size_t flags)
{
	FILE* filePtr;

	pThis->m_nickName = nickName;
	pThis->m_prefixStr = prefix;
	pThis->m_flags = flags;
	pThis->m_outputFileName = outputFileName;

	filePtr = fopen(pThis->m_outputFileName, "wb");
	if (filePtr)
	{
		fclose(filePtr);
	}
}

void gi_LoggerChannel_SetFlags(gi_LoggerChannel* const pThis, size_t flags)
{
	pThis->m_flags = flags;
}

size_t gi_LoggerChannel_GetFlags(gi_LoggerChannel* const pThis)
{
	return pThis->m_flags;
}

void gi_LoggerChannel_ChangeFlags(gi_LoggerChannel* const pThis, const size_t flags, const GI_BOOL enable)
{
	const size_t currentFlags = pThis->m_flags;
	const size_t newFlags = (currentFlags & ~flags) | (enable ? flags : 0);
	pThis->m_flags = newFlags;
}

void gi_LoggerChannel_Error(gi_LoggerChannel* const pThis, const char* const fmt, ...)
{
	va_list argPtr;
	va_start(argPtr, fmt);

	gi_Logger_SetErrorStatus();
	gi_LoggerChannel_InternalOutput(pThis, fmt, &argPtr);
}

void gi_LoggerChannel_Print(gi_LoggerChannel* const pThis, const char* const fmt, ...)
{
	va_list argPtr;
	va_start(argPtr, fmt);

	gi_LoggerChannel_InternalOutput(pThis, fmt, &argPtr);
}

