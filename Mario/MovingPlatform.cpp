#include "MovingPlatform.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "MarioGame.h"
#include "Mario.h"

MovingPlatform::MovingPlatform()
{
    renderOrder = 1001;
}

void MovingPlatform::InitResource()
{
    if (animations.size() < 1) {
        animations["Default"] = AnimationManager::GetInstance()->Get("ani-platform")->Clone();
    }
}

void MovingPlatform::Update()
{
}

void MovingPlatform::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
    vector<shared_ptr<GameObject>> mro;
    mro.push_back(MarioGame::GetInstance()->GetMario());

    shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

    collisionCal->CalcPotentialCollisions(&mro, false);
}

void MovingPlatform::PositionUpdate()
{
    Position += Velocity * MarioGame::Time().ElapsedGameTime;
}

void MovingPlatform::StatusUpdate()
{
    vector<shared_ptr<CollisionResult>> result = collisionCal->GetLastResults();
    if (result.size() > 0) {
        for each (shared_ptr<CollisionResult> rs in result)
        {
            if (rs->SAABBResult.Direction == Direction::Bottom) {
                Velocity.x = 0;
                Velocity.y = 0.2;

                Vec2 veloc = rs->Object->GetVelocity();
                veloc.y = Velocity.y * 3 + 1;
                rs->Object->SetVelocity(veloc);
            }
        }
    }
}

void MovingPlatform::Render(D3DCOLOR overlay)
{
    InitResource();

    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

    Animation animation = this->animations["Default"];

    animation->GetTransform()->Position = GetPosition() - cam + size / 2;
    animation->Render(overlay);
}

ObjectType MovingPlatform::GetObjectType()
{
    return MEntityType::MovingPlatform;
}

RectF MovingPlatform::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool MovingPlatform::IsGetThrough(GameObject& object, Direction direction)
{
    return !MEntityType::IsMario(object.GetObjectType()) || direction == Direction::Bottom;
}

float MovingPlatform::GetDamageFor(GameObject& object, Direction direction)
{
    return 0.0f;
}

shared_ptr<MovingPlatform> MovingPlatform::CreateMovingPlatform(Vec2 pos, Vec2 size, MapProperties& props)
{
    shared_ptr<MovingPlatform> platform = make_shared<MovingPlatform>();
    platform->SetCollisionCalculator(make_shared<CollisionCalculator>(platform));

    platform->SetPosition(pos);
    platform->size = size;

    platform->Velocity.x = props.GetFloat("VelocityX");
    platform->Velocity.y = props.GetFloat("VelocityY");

    return platform;
}
