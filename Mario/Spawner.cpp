#include "Spawner.h"
#include "MEntityType.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "Coin.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MarioGame.h"
#include "FlyingRedKoopas.h"
#include "BoomerangBrother.h"

Spawner::Spawner()
{
	spawnTimer.Reset();
	spawnTimer.Stop();
	this->SetCollidibility(false);
}

shared_ptr<GameObject> Spawner::GetEntity()
{
	if (type.compare(MEntityType::Goomba.ToString()) == 0) {
		return (Goomba::CreateGoomba(Position));
	}
	if (type.compare(MEntityType::RedGoomba.ToString()) == 0) {
		return (RedGoomba::CreateRedGoomba(Position));
	}
	if (type.compare(MEntityType::Koopas.ToString()) == 0) {
		return (Koopas::CreateKoopas(Position));
	}
	if (type.compare(MEntityType::KoopasJumping.ToString()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(Position);
		kp->SetPower(make_shared<JumpingKoopas>(kp));
		return (kp);
	}
	if (type.compare(MEntityType::RedKoopas.ToString()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(Position);
		kp->SetPower(make_shared<DefRedKoopas>(kp));
		return (kp);
	}
	if (type.compare(MEntityType::RedKoopasFlying.ToString()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(Position);
		kp->SetPower(make_shared<FlyingRedKoopas>(kp, Position, props.GetFloat("Range", 196.0f)));
		return (kp);
	}
	if (type.compare(MEntityType::Venus.ToString()) == 0) {
		return (Venus::CreateVenus(Position));
	}
	if (type.compare(MEntityType::RedVenus.ToString()) == 0) {
		return (RedVenus::CreateRedVenus(Position));
	}
	if (type.compare(MEntityType::BoomerangBrother.ToString()) == 0) {
		return (BoomerangBrother::CreateBoomerangBrother(Position, props));
	}
	if (type.compare(MEntityType::Piranha.ToString()) == 0) {
		return (Piranha::CreatePiranha(Position));
	}
	if (type.compare(MEntityType::Coin.ToString()) == 0) {
		return (Coin::CreateCoin(Position, CoinState::COIN));
	}
}

void Spawner::Spawn()
{
	if (spawnTimer.IsRunning() && spawnTimer.Elapsed() < spawnDelay) return;
	if (respawnLimit <= 0) return;

	if (entities.size() == spawnLimit) {
		entities.erase(remove_if(entities.begin(), entities.end(), [](const shared_ptr<GameObject>& obj) {
			return !obj->IsActive();
			}), entities.end());
	}
	if (entities.size() == spawnLimit) return;

	shared_ptr<Mario> mario = MarioGame::GetInstance()->GetMario();
	shared_ptr<GameObject> obj = GetEntity();
	
	if (mario->GetPosition().x < Position.x) {
		obj->SetFacing(-1);
		obj->GetVelocity().x = -abs(obj->GetVelocity().x);
	}
	else {
		obj->SetFacing(1);
		obj->GetVelocity().x = abs(obj->GetVelocity().x);
	}

	entities.push_back(obj);
	SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(obj);
	respawnLimit--;
	spawnTimer.Restart();
}

void Spawner::OnGetInCamera()
{
	Spawn();
}

void Spawner::PositionUpdate()
{
}

void Spawner::RestoreCollision()
{
}

void Spawner::PositionLateUpdate()
{
}

void Spawner::CollisionDoubleFilter()
{
}

void Spawner::StatusUpdate()
{
}

bool Spawner::HasCollideWith(DWORD id)
{
	return false;
}

void Spawner::FinalUpdate()
{
}

void Spawner::Update()
{
	if (!autoSpawn) return;
	Spawn();
}

void Spawner::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void Spawner::Render(D3DXCOLOR overlay)
{
}

ObjectType Spawner::GetObjectType()
{
	return MEntityType::Spawner;
}

RectF Spawner::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Spawner::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Spawner::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<Spawner> Spawner::CreateSpawner(Vec2 fixedPos, MapProperties properties)
{
	shared_ptr<Spawner> spawner = make_shared<Spawner>();
	spawner->SetPosition(Vec2(fixedPos.x, fixedPos.y));

	spawner->autoSpawn = properties.GetBool("AutoSpawn", false);
	spawner->respawnLimit = properties.GetInt("RespawnLimit", 1);
	spawner->spawnDelay = properties.GetInt("SpawnDelay", 500);
	spawner->type = properties.GetText("EntityType", "Goomba");
	spawner->spawnLimit = properties.GetInt("SpawnLimit", 1);
	spawner->respawnLimit += spawner->spawnLimit;
	spawner->props = properties;

	return spawner;
}