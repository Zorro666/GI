#ifndef GI_LOGGER_HH
#define GI_LOGGER_HH

#include <stdarg.h>
#include <stdio.h>

#include "gi.h"
#include "gi_LoggerChannel.h"

#define GI_FATAL_ERROR gi_Logger_FatalError
#define GI_ERROR gi_Logger_Error
#define GI_WARNING gi_Logger_Warning
#define GI_LOG gi_Logger_Log

enum gi_LogChannel {	GI_CHANNEL_FATAL_ERROR = 0,
											GI_CHANNEL_ERROR = 1,
											GI_CHANNEL_WARNING = 2,
											GI_CHANNEL_LOG = 3,
											GI_CHANNEL_NUM_DEFAULT_CHANNELS = 4
									};
enum gi_LogFlags { FLAGS_DEFAULT = GI_LOGGERCHANNEL_FLAGS_CONSOLE_OUTPUT | GI_LOGGERCHANNEL_FLAGS_FILE_OUTPUT };

void gi_Logger_Init(void);
void gi_Logger_Shutdown(void);

GI_BOOL gi_Logger_GetErrorStatus(void);
void gi_Logger_ClearErrorStatus(void);

gi_LoggerChannel* gi_Logger_CreateChannel(const char* const nickName, const char* const prefix, 
																					 const char* const outputFileName, const size_t flags);

gi_LoggerChannel* gi_Logger_GetDefaultChannel(const int channel);

void gi_Logger_SetChannelOutputFileName(const int channel, const char* const fname);
void gi_Logger_SetChannelFlags(const int channel, const size_t flags);
unsigned int gi_Logger_GetChannelFlags(const int channel);

void gi_Logger_SetFatal(const int channel, const GI_BOOL fatal);
void gi_Logger_SetConsoleOutput(const int channel, const GI_BOOL consoleOutput);
void gi_Logger_SetFileOutput(const int channel, const GI_BOOL fileOutput);

void gi_Logger_FatalError(const char* const fmt, ...);
void gi_Logger_Error(const char* const fmt, ...);
void gi_Logger_Warning(const char* const fmt, ...);
void gi_Logger_Log(const char* const fmt, ...);

void gi_Logger_SetErrorStatus(void);

#endif /* #ifdef GI_LOGGER_HH */
