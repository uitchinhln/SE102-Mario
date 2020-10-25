#include "Sprite.h"
#include "Game.h"

CSprite::CSprite(string id, int left, int top, int width, int height, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->pivot = D3DXVECTOR3(xPivot, yPivot, 0);
	this->texture = texture;
}

void CSprite::Draw(float x, float y, Transform& transform, D3DCOLOR overlay)
{
	RECT r;
	r.left = this->left;
	r.top = this->top;
	r.right = this->left + this->width;
	r.bottom = this->top + this->height;

	D3DXVECTOR3 pv = D3DXVECTOR3((transform.Scale.x < 0 ? 0 : pivot.x) * abs(transform.Scale.x), 
		(transform.Scale.y < 0 ? 0 : pivot.y) * abs(transform.Scale.y), 0);

	CGame::GetInstance()->GetGraphic().Draw(x, y, pv, this->texture, r, transform, overlay);
}
