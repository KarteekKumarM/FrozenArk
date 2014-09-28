#include "w_main.h"
#include "r_main.h"
#include "console.h"

// forward declarations
void W_Shutdown();

LRESULT CALLBACK W_WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		W_Shutdown();
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void W_InitalizeWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// TODO: optional cursor
	wc.lpszClassName = WIN_CLASSNAME;		// TODO: config string file
	wc.lpfnWndProc = W_WindowProc;

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
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

	R_Init(hWnd,640,480);

	ShowWindow(hWnd, nCmdShow);

	C_PrintLn(CON_CHANNEL_SYSTEM,"W_InitalizeWindow() complete");
}

void W_EnterMessageLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke message to right format
			TranslateMessage(&msg);

			// send message to WindowsProc callback
			DispatchMessage(&msg);
			// check if its time to quit
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}

		R_Frame();
	}
}

void W_Shutdown()
{
	R_Shutdown();
	C_PrintLn(CON_CHANNEL_SYSTEM, "R_Shutdown() complete");
}