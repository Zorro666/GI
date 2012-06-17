#include <string.h>
#include <malloc.h>

#include "gi_Logger.h"

#define GI_LOG_MAX_NUM_CHANNELS 128

#define DEFAULT_LOG_OUTPUT_FILE "log.txt"

gi_LoggerChannel** s_channelPtrs = NULL;
GI_BOOL s_errorFlag = GI_FALSE;

gi_LoggerChannel* s_defaultChannels[GI_CHANNEL_NUM_DEFAULT_CHANNELS];

/* ///////////////////////////////////////////////////////////////////////////////////////////////// */
/* // */
/* // Public data and functions */
/* // */
/* ///////////////////////////////////////////////////////////////////////////////////////////////// */

void gi_Logger_Init(void)
{
	size_t defaultFlags;
	s_channelPtrs = (gi_LoggerChannel**)malloc(sizeof(gi_LoggerChannel*)*GI_LOG_MAX_NUM_CHANNELS);
	if (s_channelPtrs == NULL)
	{
		return;
	}
	memset(s_channelPtrs, 0, sizeof(gi_LoggerChannel*)*GI_LOG_MAX_NUM_CHANNELS);
	memset(s_defaultChannels, 0, sizeof(gi_LoggerChannel*)*GI_CHANNEL_NUM_DEFAULT_CHANNELS);

	/*Create default channels*/
	defaultFlags = FLAGS_DEFAULT;

	s_defaultChannels[GI_CHANNEL_FATAL_ERROR] = gi_Logger_CreateChannel("FATAL_ERROR", "FATAL_ERROR: ", DEFAULT_LOG_OUTPUT_FILE, 
										 																		 defaultFlags | GI_LOGGERCHANNEL_FLAGS_FATAL);

	s_defaultChannels[GI_CHANNEL_ERROR] = gi_Logger_CreateChannel("ERROR", "ERROR: ", DEFAULT_LOG_OUTPUT_FILE, defaultFlags);
	s_defaultChannels[GI_CHANNEL_WARNING] = gi_Logger_CreateChannel("WARNING", "WARNING: ", DEFAULT_LOG_OUTPUT_FILE, defaultFlags);
	s_defaultChannels[GI_CHANNEL_LOG] = gi_Logger_CreateChannel("LOG", "", DEFAULT_LOG_OUTPUT_FILE, defaultFlags);
}

void gi_Logger_Shutdown(void)
{
	/*Loop over channels closing them down*/
}

GI_BOOL gi_Logger_GetErrorStatus(void)
{
	return s_errorFlag;
}

void gi_Logger_ClearErrorStatus(void)
{
	s_errorFlag = GI_FALSE;
}

void gi_Logger_SetErrorStatus(void)
{
	s_errorFlag = GI_TRUE;
}

gi_LoggerChannel* gi_Logger_GetDefaultChannel(const int channel)
{
	if ((channel < 0) || (channel >= GI_CHANNEL_NUM_DEFAULT_CHANNELS))
	{
		return NULL;
	}
	return s_defaultChannels[channel];
}

unsigned int gi_Logger_GetChannelFlags(const int channel)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		return gi_LoggerChannel_GetFlags(channelPtr);
	}
	return 0;
}

void gi_Logger_SetChannelFlags(const int channel, const size_t flags)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		gi_LoggerChannel_SetFlags(channelPtr, flags);
	}
}

void gi_Logger_SetFatal(const int channel, const GI_BOOL fatal)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		gi_LoggerChannel_ChangeFlags(channelPtr, GI_LOGGERCHANNEL_FLAGS_FATAL, fatal);
	}
}

void gi_Logger_SetConsoleOutput(const int channel, const GI_BOOL consoleOutput)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		gi_LoggerChannel_ChangeFlags(channelPtr, GI_LOGGERCHANNEL_FLAGS_CONSOLE_OUTPUT, consoleOutput);
	}
}

void gi_Logger_SetFileOutput(const int channel, const GI_BOOL fileOutput)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		gi_LoggerChannel_ChangeFlags(channelPtr, GI_LOGGERCHANNEL_FLAGS_FILE_OUTPUT, fileOutput);
	}
}

void gi_Logger_SetChannelOutputFileName(const int channel, const char* const fname)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(channel);
	if (channelPtr)
	{
		gi_LoggerChannel_SetOutputFileName(channelPtr, fname);
	}
}

void gi_Logger_FatalError(const char* const fmt, ...)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(GI_CHANNEL_FATAL_ERROR);
	va_list argPtr;
	va_start(argPtr, fmt);

	gi_Logger_SetErrorStatus();
	if (channelPtr)
	{
		gi_LoggerChannel_InternalOutput(channelPtr, fmt, &argPtr);
	}
}

void gi_Logger_Error(const char* const fmt, ...)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(GI_CHANNEL_ERROR);
	va_list argPtr;
	va_start(argPtr, fmt);

	gi_Logger_SetErrorStatus();
	if (channelPtr)
	{
		gi_LoggerChannel_InternalOutput(channelPtr, fmt, &argPtr);
	}
}

void gi_Logger_Warning(const char* const fmt, ...)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(GI_CHANNEL_WARNING);
	va_list argPtr;
	va_start(argPtr, fmt);

	if (channelPtr)
	{
		gi_LoggerChannel_InternalOutput(channelPtr, fmt, &argPtr);
	}
}

void gi_Logger_Log(const char* const fmt, ...)
{
	gi_LoggerChannel* const channelPtr = gi_Logger_GetDefaultChannel(GI_CHANNEL_LOG);
	va_list argPtr;
	va_start(argPtr, fmt);

	if (channelPtr)
	{
		gi_LoggerChannel_InternalOutput(channelPtr, fmt, &argPtr);
	}
}

gi_LoggerChannel* gi_Logger_CreateChannel(const char* const nickName, const char* const prefix, 
																						 const char* const outputFileName, const size_t flags)
{
	size_t channel;
	for (channel = 0; channel < GI_LOG_MAX_NUM_CHANNELS; channel++)
	{
		gi_LoggerChannel* const channelPtr = s_channelPtrs[channel];
		if (channelPtr == NULL)
		{
			gi_LoggerChannel* const newChannel = (gi_LoggerChannel* const)malloc(sizeof(gi_LoggerChannel));
			gi_LoggerChannel_Init(newChannel, nickName, prefix, outputFileName, flags);
			s_channelPtrs[channel] = newChannel;
			return newChannel;
		}
	}
	return NULL;
}


