#include "EndPortal.h"
#include "MEntityType.h"
#include "Mario.h"
#include "Game.h"

void EndPortal::Update()
{
    Portal::Update();

    if (!hasMario) return;
    mario->GetDistance() = VECTOR_0;
    mario->SetVelocity(VECTOR_0);

    if (direction == Direction::Top || direction == Direction::Bottom) {
        RectF hitbox = mario->GetHitBox();
        float marioWidth = hitbox.right - hitbox.left;
        mario->GetPosition().x = this->Position.x - (marioWidth - size.x) / 2;

        mario->SetWarpState(WarpStates::VERTICAL);

        if (direction == Direction::Top) {
            mario->GetPosition().y += 0.1 * CGame::Time().ElapsedGameTime;
        }
        else {
            mario->GetPosition().y -= 0.1 * CGame::Time().ElapsedGameTime;
        }
    }
    else {

    }

    Portal::Update();
    if (!hasMario) {
        mario->SetWarpState(WarpStates::NONE);
    }
}

ObjectType EndPortal::GetObjectType()
{
    return MEntityType::EndPortal;
}

shared_ptr<EndPortal> EndPortal::CreatePortal(Vec2 fixedPos, Vec2 size, MapProperties properties)
{
    shared_ptr<EndPortal> portal = make_shared<EndPortal>();

    int drct = properties.GetInt("Direction", 1);
    switch (drct)
    {
    case 1:
        portal->direction = Direction::Top;
        break;
    case 2:
        portal->direction = Direction::Bottom;
        break;
    case 3:
        portal->direction = Direction::Left;
        break;
    case 4:
        portal->direction = Direction::Right;
        break;
    }

    portal->size - size;
    portal->Position = fixedPos;
    portal->hitbox = RectF(fixedPos.x, fixedPos.y, fixedPos.x + size.x, fixedPos.y + size.y);

    return portal;
}
