#pragma once
#include "Particle.h"

class SpriteParticle :
    public Particle
{
public:
	unordered_map<string, Sprite> Sprites;
	Transform Transform;

public:
	virtual void Render(D3DXCOLOR overlay = D3DXCOLOR(255, 255, 255, 255)) override;

	virtual void Render(int runTime, int totalTime, D3DXCOLOR overlay = D3DXCOLOR(255, 255, 255, 255)) override;

	virtual bool HasResource() override;
};

