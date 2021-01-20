#include "CardFX.h"
#include "AnimationManager.h"

CardFX::CardFX(Vec2 pos, Vec2 velocity, CardType type) : IEffect(pos, 20000)
{
	Particle* particle = new Particle();
	particle->Position = pos;
	particle->Size = Vec2(48, 48);
	particle->Velocity = velocity;

	this->particles.push_back(particle);
	this->type = type;
}

void CardFX::InitResource(Particle* particle)
{
	switch (type)
	{
	case CardType::FLOWER:
		particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-goal-flower")->Clone();
		break;
	case CardType::MUSHROOM:
		particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-goal-mushroom")->Clone();
		break;
	case CardType::STAR:
		particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-goal-star")->Clone();
		break;
	}
}