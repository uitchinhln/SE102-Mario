#include "PSwitch.h"
#include "MEntityType.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Coin.h"

PSwitch::PSwitch()
{
    DWORD dt = CGame::Time().ElapsedGameTime;
    this->SetFacing(-1);

    this->GetGravity() = 0;
    this->Velocity = Vec2(0, 0);

    this->Distance = Velocity * (float)dt;
}

void PSwitch::InitResource()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-switch-block");
        this->animations["Pressed"] = AnimationManager::GetInstance()->Get("ani-switch-block-pressed");
    }
}

void PSwitch::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
    if (Pressed) return;
    GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void PSwitch::PositionUpdate()
{
    UpdatedDistance = collisionCal->GetClampDistance();
    Position += UpdatedDistance;
}

void PSwitch::StatusUpdate()
{
    if (Pressed) return;
    vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

    if (coResult.size() > 0) {
        for each (shared_ptr<CollisionResult> coll in coResult)
        {
            if (MEntityType::IsMario(coll->Object->GetObjectType())) {
                Pressed = true;
                Position.y += 30;
                size.y = 15;

                vector<shared_ptr<GameObject>> objects = SceneManager::GetInstance()->GetActiveScene()->GetObjects();
                for each (shared_ptr<GameObject> obj in objects)
                {
                    if (obj->GetObjectType() == MEntityType::Brick) {
                        shared_ptr<Coin> coin = dynamic_pointer_cast<Coin>(obj);
                        coin->State() = CoinState::COIN;
                    }
                }
            }
        }
    }
}

void PSwitch::Update()
{
    DWORD dt = CGame::Time().ElapsedGameTime;
    shared_ptr<Mario> player;

    switch (movingStep)
    {
    case 0:
        if (Position.y + size.y <= rootPos.y) {
            Position.y = rootPos.y - size.y;
            Velocity.y = 0;
            movingStep = 1;
        }
        else {
            Velocity.y -= MUSHROOM_GROWUP_SPEED * CGame::Time().ElapsedGameTime;
        }
        break;
    default:
        break;
    }

    GetVelocity().y += GetGravity() * (float)dt;
    GetDistance() = GetVelocity() * (float)dt;
}

void PSwitch::FinalUpdate()
{
    Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
    facing = Velocity.x > 0 ? 1 : -1;
    collisionCal->Clear();
}

void PSwitch::Render()
{
    InitResource();

    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

    Animation animation = !Pressed ? this->animations["Default"] : this->animations["Pressed"];

    animation->GetTransform()->Position = GetPosition() - cam;
    animation->Render();
}

ObjectType PSwitch::GetObjectType()
{
    return MEntityType::PSwitch;
}

RectF PSwitch::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool PSwitch::IsGetThrough(GameObject& object, Direction direction)
{
    return true;
}

float PSwitch::GetDamageFor(GameObject& object, Direction direction)
{
    return 0.0f;
}

shared_ptr<PSwitch> PSwitch::CreatePSwitch(Vec2 pos)
{
    shared_ptr<PSwitch> mushroom = make_shared<PSwitch>();
    mushroom->SetCollisionCalculator(make_shared<CollisionCalculator>(mushroom));
    mushroom->SetPosition(pos);
    mushroom->rootPos = pos;
    return mushroom;
}
