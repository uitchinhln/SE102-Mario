#include "TransformFX.h"
#include "AnimationManager.h"

TransformFX::TransformFX(Vec2 pos, shared_ptr<Mario> mario) : MarioFX(pos, mario)
{
	Particle* particle = new Particle();
	particle->Size = Vec2(45, 81);
	particle->Velocity = VECTOR_0;

	this->particles.push_back(particle);
	this->SetPlayTimeLeft(600);
}

void TransformFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-mario-damaged")->Clone();
}
