#include "RedMushroom.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Game.h"
#include "AnimationManager.h"
#include "Mario.h"

RedMushroom::RedMushroom()
{
    DWORD dt = CGame::Time().ElapsedGameTime;
    this->SetFacing(-1);

    this->GetGravity() = 0;
    this->Velocity = Vec2(0, Gravity);

    this->Distance = Velocity * (float)dt;
}

void RedMushroom::InitResource()
{
    if (this->animations.size() < 1) {
        this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-super-mushroom");
    }
}

void RedMushroom::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
    GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void RedMushroom::PositionUpdate()
{
    UpdatedDistance = collisionCal->GetClampDistance();
    Position += UpdatedDistance;
}

void RedMushroom::StatusUpdate()
{
    vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

    if (coResult.size() > 0) {
        Vec2 jet = collisionCal->GetJet();

        if (jet.x != 0) Velocity.x = -Velocity.x;
        if (jet.y != 0) Velocity.y = 0;

        for each (shared_ptr<CollisionResult> coll in coResult)
        {
            if (MEntityType::IsMario(coll->Object->GetObjectType())) {
                SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
            }
        }
    }

    Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
    if (GetPosition().x < 0.3 - size.x || GetPosition().y < 0.3 - size.y || GetPosition().x > mapBound.x || GetPosition().y > mapBound.y) {
        SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
    }
}

void RedMushroom::Update()
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
    case 1:
        this->GetGravity() = MUSHROOM_GRAVITY;
        player = SceneManager::GetInstance()->GetPlayer<Mario>();
        Velocity.x = MUSHROOM_SPEED * (player->GetPosition().x < Position.x ? 1 : -1);
        movingStep = 2;
        break;
    default:        
        break;
    }

    GetVelocity().y += GetGravity() * (float)dt;
    GetDistance() = GetVelocity() * (float)dt;
}

void RedMushroom::FinalUpdate()
{
    Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
    facing = Velocity.x > 0 ? 1 : -1;
    collisionCal->Clear();
}

void RedMushroom::Render()
{
    InitResource();

    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

    Animation animation = this->animations["Default"];

    animation->GetTransform()->Position = GetPosition() - cam;
    animation->Render();
}

ObjectType RedMushroom::GetObjectType()
{
    return MEntityType::RedMushroom;
}

RectF RedMushroom::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool RedMushroom::IsGetThrough(GameObject& object, Direction direction)
{
    return true;
}

float RedMushroom::GetDamageFor(GameObject& object, Direction direction)
{
    return 0.0f;
}

shared_ptr<RedMushroom> RedMushroom::CreateRedMushroom(Vec2 pos)
{
    shared_ptr<RedMushroom> mushroom = make_shared<RedMushroom>();
    mushroom->SetCollisionCalculator(make_shared<CollisionCalculator>(mushroom));
    mushroom->SetPosition(pos);
    mushroom->rootPos = pos;
    return mushroom;
}
