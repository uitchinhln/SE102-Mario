#pragma once
#include "Utils.h"

class Viewport
{
public:
	Viewport(Vec2 pos, Vec2 size);

	virtual D3D10_VIEWPORT GetD3DViewport();

	virtual void GetD3DViewport(D3D10_VIEWPORT& output);

	virtual RECT GetScissorRect();

protected:
	D3D10_VIEWPORT d3dvp;

	RECT scissorRect;
};

