#include "BrickDebrisFX.h"
#include "AnimationManager.h"

BrickDebrisFX::BrickDebrisFX(Vec2 pos) : IEffect(pos, 5000)
{
	for (int i = 0; i < 4; i++)
	{
		Particle* particle = new Particle();
		particle->Position = pos;
		particle->Size = Vec2(12, 12);
		particle->Gravity = 0.002f;

		switch (i)
		{
		case 0:
			particle->Velocity = Vec2(0.3, -1.3) * 0.5f;
			break;
		case 1:
			particle->Velocity = Vec2(0.3, -0.5) * 0.5f;
			break;
		case 2:
			particle->Velocity = Vec2(-0.3, -0.5) * 0.5f;
			break;
		case 3:
			particle->Velocity = Vec2(-0.3, -1.3) * 0.5f;
			break;
		}

		this->particles.push_back(particle);
	}
}

void BrickDebrisFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-brick-debris")->Clone();
	particle->Animations["Default"]->GetTransform()->Scale = Vec2(0.7f, 0.7f);
}
