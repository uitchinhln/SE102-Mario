#include "BeginPortal.h"
#include "MEntityType.h"
#include "Mario.h"
#include "Game.h"
#include "SceneManager.h"
#include "MarioGame.h"

void BeginPortal::Update()
{
    Portal::Update();

    if (!hasMario) return;
    shared_ptr<Mario> mario = MarioGame::GetInstance()->GetMario();

    WarpStates warpState = mario->GetWarpState();

    if (warpState == WarpStates::NONE) {
        if (direction == Direction::Top && mario->GetKeyboard()->IsKeyDown(DIK_DOWN)) {
            SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetFreeze(true);
            mario->SetWarpState(WarpStates::VERTICAL);
            mario->GetDistance() = VECTOR_0;

            RectF hitbox = mario->GetHitBox();
            float marioWidth = hitbox.right - hitbox.left;
            mario->GetPosition().x = this->Position.x - (marioWidth - size.x) / 2;
        }

        if (direction == Direction::Bottom && mario->GetKeyboard()->IsKeyDown(DIK_UP)) {
            SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetFreeze(true);
            mario->SetWarpState(WarpStates::VERTICAL);
            mario->GetDistance() = VECTOR_0;

            RectF hitbox = mario->GetHitBox();
            float marioWidth = hitbox.right - hitbox.left;
            mario->GetPosition().x = this->Position.x - (marioWidth - size.x) / 2;
        }
    }
    else {
        if (direction == Direction::Top || direction == Direction::Bottom) {
            RectF hitbox = mario->GetHitBox();
            float marioWidth = hitbox.right - hitbox.left;
            mario->GetPosition().x = this->Position.x - (marioWidth - size.x) / 2;
            mario->GetDistance() = VECTOR_0;

            if (direction == Direction::Top) {
                hitbox.top += 0.1 * CGame::Time().ElapsedGameTime;
                mario->GetPosition().y = hitbox.top;

                if (hitbox.top > Position.y + 1) {
                    mario->SetPosition(destination);
                    shared_ptr<GameObject> passenger = mario->GetInhand();
                    if (passenger) {
                        passenger->SetPosition(destination);
                        SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(passenger);
                    }

                    SceneManager::GetInstance()->GetActiveScene()->GetCamera()->ActiveRegion(cameraRegionId);
                    SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetFreeze(false);
                }
            }
            else {
                hitbox.top -= 0.1 * CGame::Time().ElapsedGameTime;
                mario->GetPosition().y = hitbox.top;

                if (hitbox.bottom < this->hitbox.bottom - 1) {
                    mario->SetPosition(destination);
                    shared_ptr<GameObject> passenger = mario->GetInhand();
                    if (passenger) {
                        passenger->SetPosition(destination);
                        SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(passenger);
                    }

                    SceneManager::GetInstance()->GetActiveScene()->GetCamera()->ActiveRegion(cameraRegionId);
                    SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetFreeze(false);
                }
            }
        }
        if (direction == Direction::Left || direction == Direction::Right) {
        }
    }
}

ObjectType BeginPortal::GetObjectType()
{
    return MEntityType::BeginPortal;
}

shared_ptr<BeginPortal> BeginPortal::CreatePortal(Vec2 fixedPos, Vec2 size, MapProperties properties)
{
    shared_ptr<BeginPortal> portal = make_shared<BeginPortal>();

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

    portal->cameraRegionId = properties.GetInt("CameraRegion", 1);

    portal->destination.x = properties.GetFloat("DestinationX", 0.0f);
    portal->destination.y = properties.GetFloat("DestinationY", 0.0f);

    return portal;
}
