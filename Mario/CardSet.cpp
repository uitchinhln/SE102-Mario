#include "CardSet.h"
#include "SceneManager.h"
#include "Mario.h"

void CardSet::Add(Vec2 pos, Vec2 size)
{
	CardSlot* cardSlot = new CardSlot(CardType::Empty);
	cardSlot->Position = pos;
	cardSlot->Size = size;

	cards.push_back(cardSlot);
}

void CardSet::Update()
{
	if (data == nullptr) {
		data = SceneManager::GetInstance()->GetPlayer<Mario>()->GetPlayerData();
	};
	if (data == nullptr) return;

	int i = 0;

	for each (CardSlot * var in cards)
	{
		if (i < data->Cards.size()) {
			var->SetAndAutoFlick(data->Cards[i], 2500, 800);
		}
		var->Update();
		i++;
	}
}

void CardSet::Render()
{
	for each (CardSlot * var in cards)
	{
		var->Render();
	}
}

CardSet::~CardSet()
{
	for each (CardSlot* var in cards)
	{
		delete var;
	}
}
