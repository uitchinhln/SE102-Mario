#include "MainUI.h"

MainUI::MainUI()
{
	camPos = Vec2(0, 0);
	camSize = Vec2(769, 579);

	hudPos = Vec2(0, 579);
	hudSize = Vec2(769, 142);
	this->camera = make_shared<Camera>(camPos, camSize);
}

void MainUI::Update()
{
}

void MainUI::Render()
{
}
