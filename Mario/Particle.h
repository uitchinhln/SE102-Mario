#pragma once
#include "Utils.h"
#include "Animation.h"

class Particle
{
public:
	unordered_map<string, shared_ptr<CAnimation>> Animations;

	Vec2 Position;

	Vec2 Velocity;

	Vec2 Size;

	float Gravity = 0;

public:
	virtual void Update();

	virtual void Render(D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	virtual void Render(int runTime, int totalTime, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	virtual RectF GetBoundingBox();

	virtual bool HasResource();

};

