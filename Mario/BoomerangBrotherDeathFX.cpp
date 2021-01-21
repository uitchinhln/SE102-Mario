#include "BoomerangBrotherDeathFX.h"
#include "SpriteManager.h"
#include "AnimationManager.h"

BoomerangBrotherDeathFX::BoomerangBrotherDeathFX(Vec2 pos, Vec2 velocity) : IEffect(pos, 2000)
{
	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(49, 72);
	particle->Velocity = velocity;
	particle->Gravity = 0.002f;

	this->particles.push_back(particle);
}

void BoomerangBrotherDeathFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-boomerang-brother-death");
	particle->Animations["Default"]->GetTransform()->Scale.y = -1;
}
