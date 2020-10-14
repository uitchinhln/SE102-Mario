#pragma once
#include "Utils.h"

class CSprite
{
	string id = "";

	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;

	LPDIRECT3DTEXTURE9 texture = nullptr;

public:
	CSprite(string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture);

	virtual void Draw(float x, float y, Vec2 scale, float rotation, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));
};

typedef CSprite* Sprite;

