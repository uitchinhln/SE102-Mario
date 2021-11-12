#pragma once
#include "ICallbackEffect.h"
#include "Texture.h"

class RectangleInFX :
    public ICallbackEffect
{
protected:
	int width = 1;
	int height = 1;
	int current = 0;

	long speed = 0;
	long playTime = 0;

	long fadeOutTime = 500;

	bool executed = false;

	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 0, 0);
	RECT r;
	Transform trans;

	Vec2 stepSize;

	LPTEXTURE texture;

public:
    RectangleInFX(RECT screenRect, long playTime, Vec2 stepSize, function<void(long)> callback);

	virtual void Update();

	virtual void Render(D3DXCOLOR overlay);
};

