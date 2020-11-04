#include "MEntityType.h"

const ObjectType MEntityType::SmallMario = ObjectType::CreateObjectType("SmallMario");
const ObjectType MEntityType::BigMario = ObjectType::CreateObjectType("BigMario");
const ObjectType MEntityType::FireMario = ObjectType::CreateObjectType("FireMario");
const ObjectType MEntityType::RaccoonMario = ObjectType::CreateObjectType("RaccoonMario");

const ObjectType MEntityType::MarioFireBall = ObjectType::CreateObjectType("MarioFireBall");
const ObjectType MEntityType::MarioTailed = ObjectType::CreateObjectType("MarioTailed");

const ObjectType MEntityType::Goomba = ObjectType::CreateObjectType("Goomba");
const ObjectType MEntityType::Koopas = ObjectType::CreateObjectType("Koopas");
const ObjectType MEntityType::KoopasCrouch = ObjectType::CreateObjectType("KoopasCrouch");
const ObjectType MEntityType::KoopasImposter = ObjectType::CreateObjectType("KoopasImposter");

bool MEntityType::IsTile(ObjectType type)
{
    bool normalTile = type == "Tile";
    return normalTile;
}

bool MEntityType::IsMario(ObjectType obj)
{
    bool isSmall = obj == SmallMario;
    bool isBig = obj == BigMario;
    bool isFire = obj == FireMario;
    bool isRaccoon = obj == RaccoonMario;

    return isSmall || isBig || isFire || isRaccoon;
}

bool MEntityType::IsMarioWeapon(ObjectType obj)
{
    bool isBullet = obj == MarioFireBall;
    bool isTail = obj == MarioTailed;
	bool isShell = obj == KoopasCrouch || obj == KoopasImposter;
	return isBullet || isTail || isShell;
}

bool MEntityType::IsEnemy(ObjectType obj)
{
    bool goomba = obj == Goomba;
    bool koopas = obj == Koopas || obj == KoopasCrouch || obj == KoopasImposter;
    return goomba || koopas;
}
