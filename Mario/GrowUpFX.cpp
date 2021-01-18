#include "GrowUpFX.h"
#include "AnimationManager.h"

GrowUpFX::GrowUpFX(Vec2 pos, shared_ptr<Mario> mario) : MarioFX(pos, mario)
{
	Particle* particle = new Particle();
	particle->Size = Vec2(45, 81);
	particle->Velocity = VECTOR_0;

	this->particles.push_back(particle);
	this->SetPlayTimeLeft(1440);
}

void GrowUpFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-mario-grow-up")->Clone();
}
