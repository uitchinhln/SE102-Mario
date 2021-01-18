#include "RedGoombaExplodeFX.h"
#include "AnimationManager.h"

RedGoombaExplodeFX::RedGoombaExplodeFX(Vec2 pos, Vec2 velocity) : IEffect(pos, 2000)
{
	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(45, 45);
	particle->Velocity = velocity;
	particle->Gravity = 0.002f;

	this->particles.push_back(particle);
}

void RedGoombaExplodeFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-red-goomba-idle")->Clone();
	particle->Animations["Default"]->GetTransform()->Scale.y = -1;
}
