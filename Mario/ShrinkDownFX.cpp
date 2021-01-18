#include "ShrinkDownFX.h"
#include "AnimationManager.h"

ShrinkDownFX::ShrinkDownFX(Vec2 pos, shared_ptr<Mario> mario) : MarioFX(pos, mario)
{
	Particle* particle = new Particle();
	particle->Size = Vec2(45, 81);
	particle->Velocity = VECTOR_0;

	this->particles.push_back(particle);
	this->SetPlayTimeLeft(1620);
}

void ShrinkDownFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-mario-shrink-down")->Clone();
}
