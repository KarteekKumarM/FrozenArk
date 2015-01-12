#include <Windows.h>
#include "w_main.h"
#include "defines.h"

// Entry point for Windows program
int WINAPI WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	W_InitalizeWindow( hInstance, nCmdShow );
	W_EnterMessageLoop();
	return 0;
}
