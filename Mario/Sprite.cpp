#include "Sprite.h"
#include "Game.h"

CSprite::CSprite(string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = texture;
}

void CSprite::Draw(float x, float y, Transform& transform, D3DCOLOR overlay)
{
	RECT r;
	r.left = this->left;
	r.top = this->top;
	r.right = this->left + this->width;
	r.bottom = this->top + this->height;

	CGame::GetInstance()->GetGraphic().Draw(x, y, this->texture, r, transform, overlay);
}
