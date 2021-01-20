#pragma once
#include "Camera.h"
#include "Hud.h"
class MainUI
{
public:
	MainUI();

	virtual void Update();
	virtual void Render();

	virtual void CreateHUD(TiXmlElement* node);

protected:

	shared_ptr<Hud> hud;

	shared_ptr<Viewport> fullView;
	shared_ptr<Viewport> worldView;
};

