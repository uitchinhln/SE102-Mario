#pragma once
#include "libs.h"
#include "GameObject.h"

class CEntity : public CGameObject
{
	Vec2 velocity;
};

typedef CEntity* Entity;

