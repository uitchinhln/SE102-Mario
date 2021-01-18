#include "GoombaDieFX.h"
#include "AnimationManager.h"

GoombaDieFX::GoombaDieFX(Vec2 position) : IEffect(position, 500)
{
	Particle* particle = new Particle();
	particle->Position = position;
	particle->Size = Vec2(45, 27);
	particle->Gravity = 0;
	
	this->particles.push_back(particle);
}

void GoombaDieFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-goomba-die")->Clone();
}
