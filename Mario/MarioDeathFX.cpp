#include "MarioDeathFX.h"
#include "AnimationManager.h"
#include "Game.h"

MarioDeathFX::MarioDeathFX(Vec2 pos) : IEffect(pos, 2000)
{
	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(36, 45);
	particle->Gravity = 0.002f;
	particle->Velocity.y = -0.8f;

	this->particles.push_back(particle);
}

void MarioDeathFX::Update()
{
	if (playtimeLeft > 1800) {
		playtimeLeft -= CGame::Time().ElapsedGameTime;
	}
	else {
		IEffect::Update();
	}
}

void MarioDeathFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-small-mario-die")->Clone();
}
