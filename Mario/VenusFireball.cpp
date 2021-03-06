#include "VenusFireball.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"
#include "Vec2Utils.h"
#include "Mario.h"
#include "MarioGame.h"

VenusFireball::VenusFireball()
{
	this->renderOrder = 64;
	this->Gravity = 0;
}

void VenusFireball::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
	}
}

void VenusFireball::Reset(RectF hitbox, int facing)
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	shared_ptr<Mario> player = MarioGame::GetInstance()->GetMario();
	Vec2 marioPos = player->GetPosition();

	this->Position.x = hitbox.left + size.x / 2;
	this->Position.y = hitbox.top + 15;
	this->SetFacing(facing);

	this->Gravity = 0;

	Vec2 directionalVector = Vec2Utils::Normalize(marioPos - Position);

	float angle = 0;
	Vec2 distance = marioPos - Position;

	if (abs(distance.x) > 48 * 6 || (0 < abs(distance.y) && abs(distance.y) < 96))
		angle = (distance.x > 0 ? 25 : 155) * (distance.y < 0 ? -1 : 1);
	else
		angle = (distance.x > 0 ? 45 : 135) * (distance.y < 0 ? -1 : 1);

	angle = angle * PI / 180.0f;
	directionalVector = Vec2Utils::Normalize(Vec2(cos(angle), sin(angle)));

	Velocity = directionalVector * FIREBALL_SPEED;

	this->Distance = Velocity * (float)dt;
	this->rootPos = Position;
}

void VenusFireball::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void VenusFireball::CollisionDoubleFilter()
{
}

void VenusFireball::PositionLateUpdate()
{
}

bool VenusFireball::HasCollideWith(DWORD id)
{
	return true;
}

void VenusFireball::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;

	if (renderOrder < 999 && (abs(rootPos.x - Position.x) > 20 || abs(rootPos.y - Position.y) > 20)) {
		renderOrder = 1000;
	}
}

void VenusFireball::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
}

void VenusFireball::Render(D3DCOLOR overlay)
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	this->animations["Default"]->GetTransform()->Position = GetPosition() - cam + size / 2;
	this->animations["Default"]->SetPlayScale(2.0f);
	this->animations["Default"]->Render(overlay);
}

ObjectType VenusFireball::GetObjectType()
{
	return MEntityType::VenusFireBall;
}

RectF VenusFireball::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool VenusFireball::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float VenusFireball::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType()))
		return 1.0f;
	return 0.0f;
}

void VenusFireball::OnLostCamera()
{
	SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
}
