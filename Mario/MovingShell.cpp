#include "MovingShell.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"

MovingShell::MovingShell(shared_ptr<Koopas> koopas, bool flip) : DefaultKoopas()
{
	this->koopas = koopas;

	this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
	this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();

	if (this->flip = flip) {
		this->animations["Move"]->GetTransform()->Scale.y = -1;
	}

	this->animations["Die"]->GetTransform()->Scale.y = -1;

	koopas->GetDestroyTimer().Stop();

	koopas->GetLiveState() = KoopasLiveStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(0, 0));

	koopas->GetDistance() = koopas->GetVelocity() * dt;

	respawnTimer.Start();
}

void MovingShell::FinalUpdate()
{
}

void MovingShell::Update()
{
}

void MovingShell::StatusUpdate()
{
}

void MovingShell::Render()
{
}

ObjectType MovingShell::GetObjectType()
{
	return MEntityType::KoopasImposter;
}

float MovingShell::GetDamageFor(IColliable& object, Direction direction)
{
	return 1.0f;
}
