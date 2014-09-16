#include <Windows.h>
#include "windows.h"

// Entry point for Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	InitalizeWindow(hInstance, nCmdShow);
	EnterMessageLoop();
	return 0;
}
