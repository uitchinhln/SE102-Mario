#include "Boomerang.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "MarioGame.h"
#include "SceneManager.h"
#include "Vec2Utils.h"

Boomerang::Boomerang()
{
    //this->renderOrder = 64;
    this->Gravity = 0;
}

void Boomerang::InitResource()
{
    if (animations.size() < 1) {
        animations["Default"] = AnimationManager::GetInstance()->Get("ani-boomerang-spin")->Clone();
    }
}

void Boomerang::Reset(RectF hitbox, int facing)
{
    Position.x = rootPos.x = hitbox.left;
    Position.y = rootPos.y = hitbox.top;

    Velocity = Vec2(0.3 * -facing, -0.2);
    this->facing = facing;
    step = 0;
    accelerate = VECTOR_0;
    maxY = -99999;
}

void Boomerang::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void Boomerang::CollisionDoubleFilter()
{
}

void Boomerang::PositionLateUpdate()
{
}

bool Boomerang::HasCollideWith(DWORD id)
{
    return true;
}

void Boomerang::Update()
{
    DWORD dt = MarioGame::Time().ElapsedGameTime;

    Vec2 vDist = Position - rootPos;

    if (step == 0) {
        if (abs(vDist.x) > 196) {
            accelerate = Vec2(0, 0.0015);
            minY = abs(vDist.y);
            step = 1;
        }
    }
    else if (step == 1) {
        if (abs(vDist.y) < minY / 2) {
            Velocity.x *= -0.5;
            accelerate = Vec2(0, -0.0012);
            step = 2;
        }
    } else if (step == 2) {
        if (vDist.y > maxY) {
            maxY = vDist.y;
        }
        else {
            accelerate = VECTOR_0;
            Velocity.y = 0;
            Velocity.x *= 2;
            step = 3;
        }
    }
    

    Velocity += accelerate * (float)dt;
    Distance = GetVelocity() * (float)dt;
}

void Boomerang::FinalUpdate()
{
    Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
}

void Boomerang::Render(D3DXCOLOR overlay)
{
    InitResource();

    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

    this->animations["Default"]->GetTransform()->Position = Position - cam + size / 2;
    this->animations["Default"]->SetPlayScale(1.0f);
    this->animations["Default"]->Render(overlay);
}

ObjectType Boomerang::GetObjectType()
{
    return MEntityType::Boomerang;
}

RectF Boomerang::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Boomerang::IsGetThrough(GameObject& object, Direction direction)
{
    return true;
}

float Boomerang::GetDamageFor(GameObject& object, Direction direction)
{
    if (MEntityType::IsMario(object.GetObjectType())) {
        return 1.0;
    }
    return 0.0f;
}

void Boomerang::OnLostCamera()
{
    active = false;
}
