#pragma once
#include "Particle.h"

class SpriteParticle :
    public Particle
{
public:
	unordered_map<string, Sprite> Sprites;
	Transform Transform;

public:
	virtual void Render() override;

	virtual void Render(int runTime, int totalTime) override;

	virtual bool HasResource() override;
};

