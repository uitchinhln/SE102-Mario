#pragma once
#include "Utils.h"
#include "Transform.h"

class CSprite
{
public:
	string id = "";

	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;

	D3DXVECTOR3 pivot;

	LPDIRECT3DTEXTURE9 texture = nullptr;

public:
	CSprite(string id, int left, int top, int width, int height, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture);

	virtual void Draw(float x, float y, Transform& transform, D3DCOLOR overlay);
};

typedef CSprite* Sprite;

