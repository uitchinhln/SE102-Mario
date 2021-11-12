#pragma once
#include "Utils.h"

class Dialog
{
public:
	virtual void Update();
	virtual void Render(D3DXCOLOR overlay);

	virtual void OnKeyUp(int key);
	virtual void OnKeyDown(int key);

protected:
	Vec2 Position;
	Vec2 Size;
};

