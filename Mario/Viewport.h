#pragma once
#include "Utils.h"

class Viewport
{
public:
	Viewport(Vec2 pos, Vec2 size);

	virtual D3DVIEWPORT9 GetD3DViewport();

	virtual void GetD3DViewport(D3DVIEWPORT9& output);

	virtual RECT GetScissorRect();

protected:
	D3DVIEWPORT9 d3dvp;

	RECT scissorRect;
};

