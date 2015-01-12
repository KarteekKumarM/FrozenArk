#include "g_input.h"

#include "console.h"
#include "assert.h"

static IDirectInputDevice* s_keyboardDirectInput;
static IDirectInputDevice* s_mouseDirectInput;

static BYTE s_currentKeyboardState[256];
static DIMOUSESTATE s_currentMouseState;

static LPDIRECTINPUT s_directInput;

#define PLAYER_COMMAND_MOVE_LEFT			( 1 << 0 )
#define PLAYER_COMMAND_MOVE_RIGHT			( 1 << 1 )
#define PLAYER_COMMAND_MOVE_FORWARD			( 1 << 2 )
#define PLAYER_COMMAND_MOVE_BACK			( 1 << 3 )

static InputBinding g_inputBinding[PLAYER_COMMAND_COUNT] =
{
	{ InputType_Keyboard, DIK_W, PLAYER_COMMAND_MOVE_FORWARD },
	{ InputType_Keyboard, DIK_S, PLAYER_COMMAND_MOVE_BACK },
	{ InputType_Keyboard, DIK_A, PLAYER_COMMAND_MOVE_LEFT },
	{ InputType_Keyboard, DIK_D, PLAYER_COMMAND_MOVE_RIGHT }
};

void G_InputInit( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;
	
	hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, ( void ** ) &s_directInput, NULL );
	assert( hr == S_OK );

	hr = s_directInput->CreateDevice( GUID_SysKeyboard, &s_keyboardDirectInput, NULL );
	assert( hr == S_OK );

	hr = s_directInput->CreateDevice( GUID_SysMouse, &s_mouseDirectInput, NULL );
	assert( hr == S_OK );

	hr = s_keyboardDirectInput->SetDataFormat( &c_dfDIKeyboard );
	assert( hr == S_OK );

	hr = s_keyboardDirectInput->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	assert( hr == S_OK );

	hr = s_mouseDirectInput->SetDataFormat( &c_dfDIMouse );
	assert( hr == S_OK );

	hr = s_mouseDirectInput->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY );
	assert( hr == S_OK );
}

static void G_ReadInputs()
{
	s_keyboardDirectInput->Acquire();
	s_mouseDirectInput->Acquire();

	s_keyboardDirectInput->GetDeviceState( sizeof( s_currentKeyboardState ), &s_currentKeyboardState );
	s_mouseDirectInput->GetDeviceState( sizeof( s_currentMouseState ), &s_currentMouseState );
}

static void G_SetPlayerCommands( PlayerState *playerState )
{
	int currentInputs = 0;
	for ( UINT i = 0; i < PLAYER_COMMAND_COUNT; i++ )
	{
		InputBinding bind = g_inputBinding[i];
		if ( s_currentKeyboardState[bind.key] )
		{
			currentInputs |= bind.cmd;
		}
	}

	playerState->commands.prevButtonInputs = playerState->commands.buttonInputs;
	playerState->commands.buttonInputs = currentInputs;
}

void G_InputFrame( PlayerState *playerState )
{
	G_ReadInputs();

	G_SetPlayerCommands( playerState );
}