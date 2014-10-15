#include "console.h"

#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

// TODO: channels
void C_Print(short channel, char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	OutputDebugStringA(buffer);

	va_end(args);
}

void C_PrintLn(short channel, char *format, ...)
{
	C_Print(channel, format);
	C_Print(channel, "\n");
}
