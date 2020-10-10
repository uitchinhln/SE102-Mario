#include "libs.h"
#include "Game.h"

#define WINDOW_CLASS_NAME L"Mario_Name"
#define MAIN_WINDOW_TITLE L"Mario_Title"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 0, 0)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAME_RATE 120


/*
	Load all game resources.
*/
void LoadResources()
{

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGame::GetInstance()->Init(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE);
	LoadResources();
	CGame::GetInstance()->Run(MAX_FRAME_RATE);

	return 0;
}