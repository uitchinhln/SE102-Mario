#include "CardSlot.h"
#include "AnimationManager.h"
#include "Game.h"

CardSlot::CardSlot(CardType type)
{
	this->cardType = type;
	this->flickDelay = -1;
	this->flickDuration = -1;
}

void CardSlot::SetCard(CardType type)
{
	this->cardType = type;
	this->flickDelay = -1;
	this->flickDuration = -1;
}

void CardSlot::SetCard(CardType type, long flickDuration, long delay)
{
	this->cardType = type;
	this->flickDelay = delay;
	this->flickDuration = flickDuration;
}

void CardSlot::SetAndAutoFlick(CardType type, long flickDuration, long delay)
{
	if (this->cardType != type && type != CardType::EMPTY) {
		this->SetCard(type, flickDuration, delay);
	}
	else {
		this->cardType = type;
	}
}

void CardSlot::InitResource()
{
	if (sprites.size() < 1) {
		sprites["EMPTY"] = SpriteManager::GetInstance()->Get("spr-empty-card-0");
		sprites["MUSHROOM"] = SpriteManager::GetInstance()->Get("spr-super-mushroom-card-0");
		sprites["FLOWER"] = SpriteManager::GetInstance()->Get("spr-fire-flower-card-0");
		sprites["STAR"] = SpriteManager::GetInstance()->Get("spr-star-man-card-0");
	}
}

void CardSlot::Update()
{
	if (flickDelay >= 0) {
		flickDelay -= CGame::Time().ElapsedGameTime;
	}
	else {
		if (flickDuration >= 0) {
			flickDuration -= CGame::Time().ElapsedGameTime;
		}
	}
}

void CardSlot::Render(D3DCOLOR overlay)
{
	InitResource();
	Sprite sprite = sprites["EMPTY"];

	switch (cardType)
	{
	case CardType::EMPTY:
		sprite = sprites["EMPTY"];
		break;
	case CardType::MUSHROOM:
		sprite = sprites["MUSHROOM"];
		break;
	case CardType::STAR:
		sprite = sprites["STAR"];
		break;
	case CardType::FLOWER:
		sprite = sprites["FLOWER"];
		break;
	}

	if (flickDelay <= 0 && flickDuration >= 0) {
		if ((flickDuration / 150) % 2) {
			sprite = sprites["EMPTY"];
		}
	}

	Vec2 pos = Position + Size / 2;
	sprite->Draw(pos.x, pos.y, trans, overlay);
}
