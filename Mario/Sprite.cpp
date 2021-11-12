#include "Sprite.h"
#include "Game.h"

CSprite::CSprite(string id, int left, int top, int width, int height, int xPivot, int yPivot, LPTEXTURE texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->pivot = D3DXVECTOR3((float)xPivot, (float)yPivot, 0);
	this->texture = texture;
}

void CSprite::Draw(float x, float y, Transform& transform, D3DXCOLOR overlay)
{
	RECT r;
	r.left = this->left;
	r.top = this->top;
	r.right = this->left + this->width;
	r.bottom = this->top + this->height;

	CGame::GetInstance()->GetGraphic().Draw(x, y, pivot, this->texture, r, transform, overlay);
}
