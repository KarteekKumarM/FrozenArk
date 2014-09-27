#include <Windows.h>


#define WIN_SHOW_CURSOR true
#define WIN_CLASSNAME L"MainWindow"
#define WIN_TITLE L"Frozen Ark"
#define WIN_STYLE WS_OVERLAPPEDWINDOW

//TODO: make all this configurable through UI
#define WIN_POS_X 300
#define WIN_POS_Y 300
#define WIN_WIDTH 640
#define WIN_HEIGHT 480

void W_InitalizeWindow(HINSTANCE hInstance, int nCmdShow);
void W_EnterMessageLoop();