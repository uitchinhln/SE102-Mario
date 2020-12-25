#include "RedVenus.h"
#include "AnimationManager.h"

RedVenus::RedVenus()
{
	this->renderOrder = 125;
	this->Gravity = 0;
	this->Velocity.x = 0;
	this->Velocity.y = 0;
	this->Distance.y = 0;
	this->Distance.y = 0;

	this->shootTimer.Start();
	this->movementTimer.Start();
}

void RedVenus::InitResource()
{
	if (animations.size() < 1) {
		this->animations["RevealHeadUp"] = AnimationManager::GetInstance()->Get("ani-red-venus-fire-trap-headup")->Clone();
		this->animations["RevealHeadDown"] = AnimationManager::GetInstance()->Get("ani-red-venus-fire-trap-headdown")->Clone();
		this->animations["IdleHeadUp"] = AnimationManager::GetInstance()->Get("ani-red-venus-fire-trap-headup-idle")->Clone();
		this->animations["IdleHeadDown"] = AnimationManager::GetInstance()->Get("ani-red-venus-fire-trap-headdown-idle")->Clone();
	}
}

shared_ptr<RedVenus> RedVenus::CreateRedVenus(Vec2 fixedPos)
{
	shared_ptr<RedVenus> venus = make_shared<RedVenus>();
	venus->SetCollisionCalculator(make_shared<CollisionCalculator>(venus));
	venus->Position = venus->rootPos = fixedPos;
	venus->InitData();
	return venus;
}
