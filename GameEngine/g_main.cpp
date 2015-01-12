#include "g_main.h"


void G_Init( HINSTANCE hInstance, HWND hWnd )
{
	G_InputInit( hInstance, hWnd );
	G_InitPlayerState();
}

void G_Frame()
{
	PlayerState *playerState = G_GetCurrentPlayerState();
	G_InputFrame( playerState );
	G_UpdatePlayerState();
}