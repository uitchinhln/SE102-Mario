#pragma once
#include "Utils.h"
#include "Viewport.h"
#include "PowerMeter.h"
#include "Mario.h"
#include "Text.h"

enum class ItemCardType
{
	Empty,
	Mushroom,
	Fireflower,
	Superstar
};


struct ItemCard
{
	ItemCardType item;
};

class Hud : 
	public Viewport
{
public:
	Hud(Vec2 pos, Vec2 size);

	virtual void Update();

	virtual void Render();

protected:
	shared_ptr<Mario> player;

	void DrawCard(ItemCard& card, int x, int y);
	Sprite panel;
	vector<ItemCard> cards;
	Sprite cardVs[4];

	Text* world, * life, * score, * coin, * timer;
	PowerMeter* powerMeter;

	int blinkTimer;
	bool blinkLastCard;
	ItemCardType lastCard;
};

