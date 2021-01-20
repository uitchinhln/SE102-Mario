#include "MEntityType.h"

const ObjectType MEntityType::SmallMario = ObjectType::CreateObjectType(1, "SmallMario");
const ObjectType MEntityType::BigMario = ObjectType::CreateObjectType(2, "BigMario");
const ObjectType MEntityType::FireMario = ObjectType::CreateObjectType(3, "FireMario");
const ObjectType MEntityType::RaccoonMario = ObjectType::CreateObjectType(4, "RaccoonMario");

const ObjectType MEntityType::MarioFireBall = ObjectType::CreateObjectType(11, "MarioFireBall");
const ObjectType MEntityType::MarioTailed = ObjectType::CreateObjectType(12, "MarioTailed");

const ObjectType MEntityType::Goomba = ObjectType::CreateObjectType(101, "Goomba");

const ObjectType MEntityType::Koopas = ObjectType::CreateObjectType(121, "Koopas");
const ObjectType MEntityType::KoopasCrouch = ObjectType::CreateObjectType(122, "KoopasCrouch");
const ObjectType MEntityType::KoopasPassenger = ObjectType::CreateObjectType(123, "KoopasPassenger");
const ObjectType MEntityType::KoopasImposter = ObjectType::CreateObjectType(124, "KoopasImposter");
const ObjectType MEntityType::KoopasJumping = ObjectType::CreateObjectType(125, "KoopasJumping");

const ObjectType MEntityType::RedKoopas = ObjectType::CreateObjectType(141, "RedKoopas");
const ObjectType MEntityType::RedKoopasCrouch = ObjectType::CreateObjectType(142, "RedKoopasCrouch");
const ObjectType MEntityType::RedKoopasPassenger = ObjectType::CreateObjectType(143, "RedKoopasPassenger");
const ObjectType MEntityType::RedKoopasImposter = ObjectType::CreateObjectType(144, "RedKoopasImposter");
const ObjectType MEntityType::RedKoopasJumping = ObjectType::CreateObjectType(145, "RedKoopasJumping");

const ObjectType MEntityType::Venus = ObjectType::CreateObjectType(161, "Venus");
const ObjectType MEntityType::VenusFireBall = ObjectType::CreateObjectType(162, "VenusFireBall");
const ObjectType MEntityType::RedVenus = ObjectType::CreateObjectType(163, "RedVenus");
const ObjectType MEntityType::Piranha = ObjectType::CreateObjectType(164, "Piranha");

const ObjectType MEntityType::RedGoomba = ObjectType::CreateObjectType(181, "RedGoomba");

const ObjectType MEntityType::EndmapReward = ObjectType::CreateObjectType(1001, "EndmapReward");
const ObjectType MEntityType::QuestionBlock = ObjectType::CreateObjectType(1002, "QuestionBlock");
const ObjectType MEntityType::Spawner = ObjectType::CreateObjectType(1003, "Spawner");

const ObjectType MEntityType::SolidBlock = ObjectType::CreateObjectType(5001, "SolidBlock");
const ObjectType MEntityType::GhostBlock = ObjectType::CreateObjectType(5002, "GhostBlock");
const ObjectType MEntityType::VoidBlock = ObjectType::CreateObjectType(5003, "Void");
const ObjectType MEntityType::Pipe = ObjectType::CreateObjectType(5004, "Pipe");

const ObjectType MEntityType::RedMushroom = ObjectType::CreateObjectType(10001, "RedMushroom");
const ObjectType MEntityType::GreenMushroom = ObjectType::CreateObjectType(10002, "GreenMushroom");
const ObjectType MEntityType::RaccoonLeaf = ObjectType::CreateObjectType(10003, "RaccoonLeaf");
const ObjectType MEntityType::FireFlower = ObjectType::CreateObjectType(10004, "FireFlower");
const ObjectType MEntityType::QuestionCoin = ObjectType::CreateObjectType(10005, "QuestionCoin");
const ObjectType MEntityType::PSwitch = ObjectType::CreateObjectType(10006, "PSwitch");

const ObjectType MEntityType::Coin = ObjectType::CreateObjectType(10011, "Coin");
const ObjectType MEntityType::Brick = ObjectType::CreateObjectType(10012, "Brick");

const ObjectType MEntityType::BeginPortal = ObjectType::CreateObjectType(20011, "BeginPortal");
const ObjectType MEntityType::EndPortal = ObjectType::CreateObjectType(20012, "EndPortal");

const ObjectType MEntityType::TinyMario = ObjectType::CreateObjectType(30001, "TinyMario");
const ObjectType MEntityType::Tree = ObjectType::CreateObjectType(30002, "Tree");
const ObjectType MEntityType::Node = ObjectType::CreateObjectType(30003, "Node");

bool MEntityType::IsTile(ObjectType type)
{
    bool normalTile = type == DEFAULT_TILE_ID;
    bool questionTile = type == QuestionBlock;
	bool solidBlock = type == SolidBlock;
	bool ghostBlock = type == GhostBlock;
	bool pipe = type == Pipe;
	bool brick = type == Brick;
    return normalTile || questionTile || solidBlock || ghostBlock || pipe || brick;
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
    bool voidblock = obj == VoidBlock;
	return isBullet || isTail || isShell || voidblock;
}

bool MEntityType::IsEnemy(ObjectType obj)
{
    bool goomba = obj == Goomba;
    bool redgoomba = obj == RedGoomba;
    bool koopas = obj == Koopas || obj == KoopasCrouch || obj == KoopasImposter || obj == KoopasJumping;
    bool redkoopas = obj == RedKoopas || obj == RedKoopasCrouch || obj == RedKoopasImposter || obj == RedKoopasJumping;
	bool venus = obj == Venus || obj == VenusFireBall || obj == RedVenus;
    bool piranha = obj == Piranha;
    bool voidblock = obj == VoidBlock;
    return goomba || redgoomba || koopas || redkoopas || venus || piranha || voidblock;
}

bool MEntityType::IsPowerUpItem(ObjectType obj)
{
    bool mushroom = obj == RedMushroom;
    bool greenmushroom = obj == GreenMushroom;
    bool leaf = obj == RaccoonLeaf;
    return mushroom || greenmushroom || leaf;
}

bool MEntityType::IsMapObject(ObjectType obj)
{
    bool isVoid = obj == VoidBlock;
    bool isGhost = obj == GhostBlock;
    bool isSolid = obj == SolidBlock;
    return isVoid || isGhost || isSolid;
}
