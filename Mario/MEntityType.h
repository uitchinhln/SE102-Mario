#pragma once
#include "ObjectType.h"
class MEntityType
{
public: 
	static const ObjectType SmallMario;
	static const ObjectType BigMario;
	static const ObjectType FireMario;
	static const ObjectType RaccoonMario;

	static const ObjectType MarioFireBall;
	static const ObjectType MarioTailed;

	static const ObjectType Goomba;
	static const ObjectType Koopas;
	static const ObjectType KoopasCrouch;

	static bool IsTile(ObjectType);

	static bool IsMario(ObjectType);

	static bool IsMarioWeapon(ObjectType);

	static bool IsEnemy(ObjectType);
};

