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
	static const ObjectType RedGoomba;

	static const ObjectType Koopas;
	static const ObjectType KoopasCrouch;
	static const ObjectType KoopasPassenger;
	static const ObjectType KoopasImposter;
	static const ObjectType KoopasJumping;

	static const ObjectType RedKoopas;
	static const ObjectType RedKoopasCrouch;
	static const ObjectType RedKoopasPassenger;
	static const ObjectType RedKoopasImposter;
	static const ObjectType RedKoopasJumping;

	static const ObjectType VenusFireBall;
	static const ObjectType Venus;
	static const ObjectType RedVenus;
	static const ObjectType Piranha;

	static const ObjectType EndmapReward;
	static const ObjectType QuestionBlock;
	static const ObjectType Spawner;

	static const ObjectType SolidBlock;
	static const ObjectType GhostBlock;
	static const ObjectType VoidBlock;
	static const ObjectType Pipe;

	static const ObjectType RedMushroom;
	static const ObjectType GreenMushroom;
	static const ObjectType RaccoonLeaf;
	static const ObjectType QuestionCoin;
	static const ObjectType PSwitch;

	static const ObjectType Coin;
	static const ObjectType Brick;


	static bool IsTile(ObjectType);

	static bool IsMario(ObjectType);

	static bool IsMarioWeapon(ObjectType);

	static bool IsEnemy(ObjectType);

	static bool IsPowerUpItem(ObjectType);
};

