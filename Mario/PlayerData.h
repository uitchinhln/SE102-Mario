#pragma once
#include "MEntityType.h"

enum class CardType {
	Empty,
	Mushroom,
	Star,
	Flower
};

class PlayerData
{
public:
	PlayerData();

	ObjectType Power = MEntityType::SmallMario;

	int Lives;

	int Coins;

	int Score;

	int World;

	int RemainingTime;

	float PMeter;

	vector<CardType> Cards;
};

