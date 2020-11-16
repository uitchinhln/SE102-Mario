#pragma once
#include "Utils.h"
#include "Viewport.h"

class Hud : 
	public Viewport
{
public:
	Hud(Vec2 pos, Vec2 size);

	virtual void Update();

	virtual void Render();

protected:
};

