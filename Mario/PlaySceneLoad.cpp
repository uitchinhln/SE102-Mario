#include "PlayScene.h"

#include "Goomba.h"
#include "Koopas.h"

#include "EndmapReward.h"
#include "QuestionBlock.h"

#include "SolidBlock.h"
#include "GhostBlock.h"
#include "VoidBlock.h"
#include "Pipe.h"

#include "Spawner.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "RedGoomba.h"
#include "Coin.h"

#include "BeginPortal.h"
#include "EndPortal.h"
#include "MovingPlatform.h"
#include "FlyingRedKoopas.h"
#include "BoomerangBrother.h"

void PlayScene::ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	//Player Spawn Point
	if (strcmp(type, "SpawnPoint") == 0) {
		RectF marioBox = mario->GetHitBox();
		mario->SetPosition(fixedPos - Vec2(0, marioBox.bottom - marioBox.top));
		camera->Position.x = props.GetFloat("cameraX");
		camera->Position.y = props.GetFloat("cameraY");
	}

	//GameObjects
	if (strcmp(type, MEntityType::Goomba.ToString().c_str()) == 0) {
		SpawnEntity(Goomba::CreateGoomba(fixedPos), props);
	}
	if (strcmp(type, MEntityType::RedGoomba.ToString().c_str()) == 0) {
		SpawnEntity(RedGoomba::CreateRedGoomba(fixedPos), props);
	}
	if (strcmp(type, MEntityType::Koopas.ToString().c_str()) == 0) {
		SpawnEntity(Koopas::CreateKoopas(fixedPos), props);
	}
	if (strcmp(type, MEntityType::KoopasJumping.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<JumpingKoopas>(kp));
		SpawnEntity(kp, props);
	}
	if (strcmp(type, MEntityType::RedKoopas.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<DefRedKoopas>(kp));
		SpawnEntity(kp, props);
	}
	if (strcmp(type, MEntityType::RedKoopasFlying.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<FlyingRedKoopas>(kp, fixedPos, props.GetFloat("Range", 196.0f)));
		SpawnEntity(kp, props);
	}
	if (strcmp(type, MEntityType::Venus.ToString().c_str()) == 0) {
		SpawnEntity(Venus::CreateVenus(fixedPos), props);
	}
	if (strcmp(type, MEntityType::RedVenus.ToString().c_str()) == 0) {
		SpawnEntity(RedVenus::CreateRedVenus(fixedPos), props);
	}
	if (strcmp(type, MEntityType::Piranha.ToString().c_str()) == 0) {
		SpawnEntity(Piranha::CreatePiranha(fixedPos), props);
	}
	if (strcmp(type, MEntityType::BoomerangBrother.ToString().c_str()) == 0) {
		SpawnEntity(BoomerangBrother::CreateBoomerangBrother(fixedPos, props), props);
	}
	if (strcmp(type, MEntityType::EndmapReward.ToString().c_str()) == 0) {
		SpawnEntity(EndmapReward::CreateEndmapReward(fixedPos), props);
	}
	if (strcmp(type, MEntityType::QuestionBlock.ToString().c_str()) == 0) {
		SpawnEntity(QuestionBlock::CreateQuestionBlock(fixedPos, props), props);
	}
	if (strcmp(type, MEntityType::Spawner.ToString().c_str()) == 0) {
		SpawnEntity(Spawner::CreateSpawner(fixedPos, props), props);
	}
	if (strcmp(type, MEntityType::Pipe.ToString().c_str()) == 0) {
		SpawnEntity(Pipe::CreatePipe(fixedPos, size, props), props);
	}
	if (strcmp(type, MEntityType::Coin.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::COIN), props);
	}
	if (strcmp(type, MEntityType::Brick.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::BRICK), props);
	}
	if (strcmp(type, MEntityType::MovingPlatform.ToString().c_str()) == 0) {
		SpawnEntity(MovingPlatform::CreateMovingPlatform(fixedPos, size, props), props);
	}

	//MapObjects
	if (strcmp(type, MEntityType::BeginPortal.ToString().c_str()) == 0) {
		SpawnEntity(BeginPortal::CreatePortal(fixedPos, size, props), props);
	}
	if (strcmp(type, MEntityType::EndPortal.ToString().c_str()) == 0) {
		SpawnEntity(EndPortal::CreatePortal(fixedPos, size, props), props);
	}
	if (strcmp(type, MEntityType::SolidBlock.ToString().c_str()) == 0) {
		SpawnEntity(SolidBlock::CreateSolidBlock(fixedPos, size), props);
	}
	if (strcmp(type, MEntityType::GhostBlock.ToString().c_str()) == 0) {
		SpawnEntity(GhostBlock::CreateGhostBlock(fixedPos, size), props);
	}
	if (strcmp(type, MEntityType::VoidBlock.ToString().c_str()) == 0) {
		SpawnEntity(VoidBlock::CreateVoidBlock(fixedPos, size), props);
	}
}