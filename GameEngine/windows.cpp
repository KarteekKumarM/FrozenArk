#include "windows.h"

void InitalizeWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// TODO: optional cursor
	wc.lpszClassName = WIN_CLASSNAME;		// TODO: config string file
	wc.lpfnWndProc = WindowProc;

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

	ShowWindow(hWnd, nCmdShow);
}

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void EnterMessageLoop()
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
	}
}