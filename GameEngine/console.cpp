#include "console.h"
#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

enum consoleEnumeration
{
	CON_CHANNEL_ENUM_ALL,
	CON_CHANNEL_ENUM_ERROR,
	CON_CHANNEL_ENUM_WARNING,
	CON_CHANNEL_ENUM_RENDER,
	CON_CHANNEL_ENUM_SYSTEM,
	CON_CHANNEL_ENUM_ASSET_DB,
	CON_CHANNEL_ENUM_COUNT
};

cassert(CON_CHANNEL_ENUM_COUNT == CON_CHANNEL_COUNT);

static char *s_consoleNames[CON_CHANNEL_COUNT] =
{
	"all",
	"error",
	"warning",
	"render",
	"system",
	"asset_db"
};

static char *s_consoleFilePaths[CON_CHANNEL_COUNT] = 
{
	"Logs/all.log",
	"Logs/error.log",
	"Logs/warning.log",
	"Logs/render.log",
	"Logs/system.log",
	"Logs/asset_db.log"
};

static FILE *s_consoleFileHandles[CON_CHANNEL_COUNT];

void Con_Init()
{
	for (UINT i = 0; i < CON_CHANNEL_COUNT; i++)
	{
		FILE *fileHandle;
		char *path = s_consoleFilePaths[i];
		fopen_s(&fileHandle, path, "w");
		s_consoleFileHandles[i] = fileHandle;
	}
}

void Con_Print(short channel, char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	short bufferSize = sizeof(buffer);
	//TODO append date-time
	vsprintf_s(buffer, bufferSize, format, args);

	channel = channel | CON_CHANNEL_ALL;
	for (UINT i = 0; i < CON_CHANNEL_COUNT; i++)
	{
		short check = 1 << i;
		if ( (check & channel) != 0)
		{
			FILE *consoleFileHandle = s_consoleFileHandles[i];
			assert(consoleFileHandle != NULL);
			fputs(buffer, consoleFileHandle);
		}
	}

	// output everything to debug
	OutputDebugStringA(buffer);

	va_end(args);
}

void Con_PrintLn(short channel, char *format, ...)
{
	// TODO : Make this one - lazzeee
	Con_Print(channel, format);
	Con_Print(channel, "\n");
}

void Con_Shutdown()
{
	for (UINT i = 0; i < CON_CHANNEL_COUNT; i++)
		fclose(s_consoleFileHandles[i]);
}