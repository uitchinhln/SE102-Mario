#pragma once
#include "MEntityType.h"
#include "GameEnum.h"

class PlayerData
{
public:
	PlayerData();

	ObjectType Power = MEntityType::SmallMario;

	int Lives;

	int Coins;

	int Score;

	int World;

	int Node;

	int RemainingTime;

	float PMeter;

	vector<CardType> Cards;
};

