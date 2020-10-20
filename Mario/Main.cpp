#include "Utils.h"
#include "MarioGame.h";
#include "DataTag.h"

void LoadResources()
{
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(new MarioGame())->Run();
	return 0;
}