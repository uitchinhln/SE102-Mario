#pragma once
#include "Camera.h"
#include "Hud.h"
class MainUI
{
public:
	MainUI();

	virtual void Update();

	virtual void Render();

protected:

	shared_ptr<Camera> camera;
	Vec2 camPos, camSize;

	shared_ptr<Hud> hud;
	Vec2 hudPos, hudSize;


};

