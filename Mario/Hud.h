#pragma once
#include "Utils.h"

class Hud
{
public:
	Hud(Vec2 pos, Vec2 size);

	virtual void Update();

	virtual void Render();

protected:
	Vec2 pos;
	Vec2 size;

	D3DVIEWPORT9 vp, oldVP;
};

