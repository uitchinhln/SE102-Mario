#include "Viewport.h"

Viewport::Viewport(Vec2 pos, Vec2 size)
{
	d3dvp.X = (unsigned long)pos.x;
	d3dvp.Y = (unsigned long)pos.y;
	d3dvp.Width = (unsigned long)size.x;
	d3dvp.Height = (unsigned long)size.y;
	
	scissorRect.left = (long)d3dvp.X;
	scissorRect.top = (long)d3dvp.Y;
	scissorRect.right = (long)(d3dvp.X + d3dvp.Width);
	scissorRect.bottom = (long)(d3dvp.Y + d3dvp.Height);
}

D3DVIEWPORT9 Viewport::GetD3DViewport()
{
	return d3dvp;
}

void Viewport::GetD3DViewport(D3DVIEWPORT9& output)
{
	output = d3dvp;
}

RECT Viewport::GetScissorRect()
{
	scissorRect.left = (long)d3dvp.X;
	scissorRect.top = (long)d3dvp.Y;
	scissorRect.right = (long)(d3dvp.X + d3dvp.Width);
	scissorRect.bottom = (long)(d3dvp.Y + d3dvp.Height);
	return scissorRect;
}
