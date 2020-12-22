#include "MEntityType.h"

const ObjectType MEntityType::SmallMario = ObjectType::CreateObjectType(1, "SmallMario");
const ObjectType MEntityType::BigMario = ObjectType::CreateObjectType(2, "BigMario");
const ObjectType MEntityType::FireMario = ObjectType::CreateObjectType(3, "FireMario");
const ObjectType MEntityType::RaccoonMario = ObjectType::CreateObjectType(4, "RaccoonMario");

const ObjectType MEntityType::MarioFireBall = ObjectType::CreateObjectType(11, "MarioFireBall");
const ObjectType MEntityType::MarioTailed = ObjectType::CreateObjectType(12, "MarioTailed");

const ObjectType MEntityType::Goomba = ObjectType::CreateObjectType(101, "Goomba");
const ObjectType MEntityType::Koopas = ObjectType::CreateObjectType(102, "Koopas");
const ObjectType MEntityType::KoopasCrouch = ObjectType::CreateObjectType(103, "KoopasCrouch");
const ObjectType MEntityType::KoopasPassenger = ObjectType::CreateObjectType(104, "KoopasPassenger");
const ObjectType MEntityType::KoopasImposter = ObjectType::CreateObjectType(105, "KoopasImposter");
const ObjectType MEntityType::KoopasJumping = ObjectType::CreateObjectType(106, "KoopasJumping");

const ObjectType MEntityType::RedKoopas = ObjectType::CreateObjectType(112, "RedKoopas");
const ObjectType MEntityType::RedKoopasCrouch = ObjectType::CreateObjectType(113, "RedKoopasCrouch");
const ObjectType MEntityType::RedKoopasPassenger = ObjectType::CreateObjectType(114, "RedKoopasPassenger");
const ObjectType MEntityType::RedKoopasImposter = ObjectType::CreateObjectType(115, "RedKoopasImposter");
const ObjectType MEntityType::RedKoopasJumping = ObjectType::CreateObjectType(116, "RedKoopasJumping");

const ObjectType MEntityType::EndmapReward = ObjectType::CreateObjectType(1001, "EndmapReward");
const ObjectType MEntityType::QuestionBlock = ObjectType::CreateObjectType(1002, "QuestionBlock");
const ObjectType MEntityType::Spawner = ObjectType::CreateObjectType(1003, "Spawner");

const ObjectType MEntityType::SolidBlock = ObjectType::CreateObjectType(5001, "SolidBlock");
const ObjectType MEntityType::GhostBlock = ObjectType::CreateObjectType(5002, "GhostBlock");
const ObjectType MEntityType::Pipe = ObjectType::CreateObjectType(5003, "Pipe");

bool MEntityType::IsTile(ObjectType type)
{
    bool normalTile = type == DEFAULT_TILE_ID;
    bool questionTile = type == QuestionBlock;
	bool solidBlock = type == SolidBlock;
	bool ghostBlock = type == GhostBlock;
	bool pipe = type == Pipe;
    return normalTile || questionTile || solidBlock || ghostBlock;
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
	bool isShell = obj == KoopasPassenger || obj == KoopasImposter;
	return isBullet || isTail || isShell;
}

bool MEntityType::IsEnemy(ObjectType obj)
{
    bool goomba = obj == Goomba;
    bool koopas = obj == Koopas || obj == KoopasCrouch || obj == KoopasImposter || obj == KoopasJumping;
    return goomba || koopas;
}
