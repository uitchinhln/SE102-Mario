#pragma once
#include "GameObject.h"
#include "StopWatch.h"

enum class PlantState
{
	Reveal,
	Hidden
};

class Plant : 
	public GameObject
{
public:
	Plant();

protected:
	Vec2 size;
	PlantState state;
	Stopwatch waitTimer, attackTimer;
};

