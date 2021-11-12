#include "Viewport.h"

Viewport::Viewport(Vec2 pos, Vec2 size)
{
	d3dvp.TopLeftX = (unsigned long)pos.x;
	d3dvp.TopLeftY = (unsigned long)pos.y;
	d3dvp.Width = (unsigned long)size.x;
	d3dvp.Height = (unsigned long)size.y;
	
	scissorRect.left = (long)d3dvp.TopLeftX;
	scissorRect.top = (long)d3dvp.TopLeftY;
	scissorRect.right = (long)(d3dvp.TopLeftX + d3dvp.Width);
	scissorRect.bottom = (long)(d3dvp.TopLeftY + d3dvp.Height);
}

D3D10_VIEWPORT Viewport::GetD3DViewport()
{
	return d3dvp;
}

void Viewport::GetD3DViewport(D3D10_VIEWPORT& output)
{
	output = d3dvp;
}

RECT Viewport::GetScissorRect()
{
	scissorRect.left = (long)d3dvp.TopLeftX;
	scissorRect.top = (long)d3dvp.TopLeftY;
	scissorRect.right = (long)(d3dvp.TopLeftX + d3dvp.Width);
	scissorRect.bottom = (long)(d3dvp.TopLeftY + d3dvp.Height);
	return scissorRect;
}
