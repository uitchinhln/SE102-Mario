#pragma once
#include "Utils.h"
#include "Transform.h"
#include "Texture.h"

class CSprite
{
public:
	string id = "";

	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;

	D3DXVECTOR3 pivot;

	LPTEXTURE texture = nullptr;

public:
	CSprite(string id, int left, int top, int width, int height, int xPivot, int yPivot, LPTEXTURE texture);

	virtual void Draw(float x, float y, Transform& transform, D3DXCOLOR overlay);
};

typedef CSprite* Sprite;

