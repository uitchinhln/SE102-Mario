#include "GoombaExplodeFX.h"
#include "AnimationManager.h"

GoombaExplodeFX::GoombaExplodeFX(Vec2 pos, Vec2 velocity) : IEffect(pos, 2000)
{
	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(45, 45);
	particle->Velocity = velocity;
	particle->Gravity = 0.002f;

	this->particles.push_back(particle);
}

void GoombaExplodeFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-goomba-idle")->Clone();
	particle->Animations["Default"]->GetTransform()->Scale.y = -1;
}

GoombaExplodeFX::~GoombaExplodeFX()
{
	DebugOut(L"Huy GoombaExplodeFX\n");
}
