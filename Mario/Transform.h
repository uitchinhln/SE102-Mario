#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2 Vec2;

class Transform
{
public:
	Vec2 Position;
	Vec2 Scale;
	float Rotation;

	Transform() {};
	Transform(Vec2 scale, float rotation) { this->Scale = scale; this->Rotation = rotation; }
};

Vec2 Vector0();

