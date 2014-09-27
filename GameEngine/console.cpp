#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

void C_Print(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	OutputDebugStringA(buffer);

	va_end(args);
}

void C_PrintLn(char *format, ...)
{
	C_Print(format);
	C_Print("\n");
}
