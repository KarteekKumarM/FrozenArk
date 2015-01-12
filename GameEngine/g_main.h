#include "g_input.h"

struct GameState
{
	UINT currTime;
	UINT prevTime;
};

void G_Init( HINSTANCE hInstance, HWND hWnd );
void G_Frame();