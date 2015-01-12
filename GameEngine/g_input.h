#include "defines.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

#include "g_player.h"

#define PLAYER_COMMAND_COUNT 4

enum InputType
{
	InputType_Keyboard,
	InputType_Mouse
};

struct InputBinding
{
	InputType inputType;
	BYTE key;
	unsigned int cmd;
};

void G_InputInit( HINSTANCE hInstance, HWND hWnd );
void G_InputFrame( PlayerState *playerState );