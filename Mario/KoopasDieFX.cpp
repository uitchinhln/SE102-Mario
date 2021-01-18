#include "KoopasDieFX.h"
#include "AnimationManager.h"

KoopasDieFX::KoopasDieFX(Vec2 pos, Vec2 velocity, string aniId) : IEffect(pos, 2000)
{
	this->Position = pos;
	this->aniId = aniId;

	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(45, 45);
	particle->Velocity = velocity;
	particle->Gravity = 0.002f;

	this->particles.push_back(particle);
}

void KoopasDieFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get(aniId)->Clone();
	particle->Animations["Default"]->GetTransform()->Scale.y = -1;
}
