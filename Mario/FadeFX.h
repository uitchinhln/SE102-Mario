#pragma once
#include "ICallbackEffect.h"
#include "StopWatch.h"

class FadeFX :
    public ICallbackEffect
{
protected:
	int width = 1;
	int height = 1;

	int step = 0;

	long timer;

	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 0, 0);
	RECT r;
	Transform trans;

	LPDIRECT3DTEXTURE9 texture;

public:
	FadeFX(RECT screenRect, long playTime, function<void(long)> callback);

	virtual void Update() override;

	virtual void Render(D3DCOLOR overlay) override;
};

