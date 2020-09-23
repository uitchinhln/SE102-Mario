#pragma once
#include "Direction.h"
#include "Vec2.h"
#include "GameObject.h"

class CEntity : CGameObject
{
	Direction direction;
	Vec2 velocity;
};

typedef CEntity* Entity;

