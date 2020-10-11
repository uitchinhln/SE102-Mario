#include "libs.h"
#include "Game.h"
#include "Animation.h"

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
	Animation ani = new CAnimation();
	ani->GetTransform()->Rotation = 10;

	Animation ani2 = ani->Clone();
	DebugOut(L"ani2 orig: %f\n", ani2->GetTransform()->Rotation);

	ani2->GetTransform()->Rotation = 20;
	DebugOut(L"ani1 edit: %f\n", ani->GetTransform()->Rotation);
	DebugOut(L"ani2 edit: %f\n", ani2->GetTransform()->Rotation);

	CGame::GetInstance()->Init(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE);
	LoadResources();
	CGame::GetInstance()->Run(MAX_FRAME_RATE);

	return 0;
}