#include "CardSet.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MarioGame.h"

void CardSet::Init()
{
	shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

	sort(cards.begin(), cards.end(), [](CardSlot* a, CardSlot* b) {
		if (a->Position.x == b->Position.x) {
			return a->Position.y < b->Position.y;
		}
		else {
			return a->Position.x < b->Position.x;
		}
		});

	int i = 0;

	for each (CardSlot * var in cards)
	{
		if (i < data->Cards.size()) {
			var->SetCard(data->Cards[i]);
		}
		i++;
	}
}

void CardSet::Add(Vec2 pos, Vec2 size)
{
	CardSlot* cardSlot = new CardSlot(CardType::EMPTY);
	cardSlot->Position = pos;
	cardSlot->Size = size;

	cards.push_back(cardSlot);
}

void CardSet::Update()
{
	shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

	int i = 0;

	for each (CardSlot * var in cards)
	{
		if (i < data->Cards.size()) {
			var->SetAndAutoFlick(data->Cards[i], 2500, 500);
		}
		var->Update();
		i++;
	}
}

void CardSet::Render(D3DCOLOR overlay)
{
	for each (CardSlot * var in cards)
	{
		var->Render(overlay);
	}
}

CardSet::~CardSet()
{
	for each (CardSlot* var in cards)
	{
		delete var;
	}
}
