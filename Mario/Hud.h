#pragma once
#include "Utils.h"
#include "Viewport.h"
#include "Mario.h"
#include "Text.h"
#include "HudPanel.h"
#include "CardSet.h"
#include "PMeter.h"

class Hud : 
	public Viewport
{
protected:
	void LoadFromTmx(string tmxPath);


public:
	Hud(string tmxPath, Vec2 pos, Vec2 size);

	virtual void Update();

	virtual void Render(D3DCOLOR overlay);

protected:
	D3DCOLOR background;

	HudPanel panel;
	CardSet cards;

	PMeter* pmeter;

	Text* world, * live, * score, * coin, * timer;
};

