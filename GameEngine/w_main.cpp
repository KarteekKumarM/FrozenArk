#include "w_main.h"
#include "r_main.h"
#include "g_main.h"
#include "console.h"
#include "c_time.h"

HWND s_hWnd;

// forward declarations
void W_Shutdown();

LRESULT CALLBACK W_WindowProc( HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	switch ( message )
	{
	case WM_DESTROY:
		W_Shutdown();
		PostQuitMessage( 0 );
		return 0;
	default:
		break;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}

static void W_CommonInit()
{
	C_InitTime();
}

static void W_CommonFrame()
{
	C_UpdateTime();
}

void W_InitalizeWindow( HINSTANCE hInstance, int nCmdShow )
{
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );	// TODO: optional cursor
	wc.lpszClassName = WIN_CLASSNAME;		// TODO: config string file
	wc.lpfnWndProc = W_WindowProc;

	RegisterClassEx( &wc );

	s_hWnd = CreateWindowEx( NULL,
		wc.lpszClassName,
		WIN_TITLE,
		WIN_STYLE,
		WIN_POS_X,
		WIN_POS_Y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	W_CommonInit();

	Con_Init();

	R_Init( s_hWnd, 640, 480 );

	ShowWindow( s_hWnd, nCmdShow );

	G_Init( hInstance, s_hWnd );

	Con_PrintLn( CON_CHANNEL_SYSTEM, "W_InitalizeWindow() complete" );
}

void W_EnterMessageLoop()
{
	MSG msg;
	while ( true )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			// translate keystroke message to right format
			TranslateMessage( &msg );

			// send message to WindowsProc callback
			DispatchMessage( &msg );
			// check if its time to quit
			if ( msg.message == WM_QUIT )
			{
				break;
			}

		}

		W_CommonFrame();
		G_Frame();
		R_Frame();
	}
}

void W_Shutdown()
{
	Con_Shutdown();
	Con_PrintLn( CON_CHANNEL_SYSTEM, "R_Shutdown() complete" );
}