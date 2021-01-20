#include "TinyMario.h"
#include "Utils.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Events.h"
#include "GameEvent.h"
#include "AnimationManager.h"
#include "Vec2Utils.h"
#include <dinput.h>
#include "Game.h"

TinyMario::TinyMario()
{
    data = SceneManager::GetInstance()->GetPlayer<Mario>()->GetPlayerData();
    graph = make_shared<Graph>();
    Position = Vec2(0, 0);
    size = Vec2(48, 48);
    renderOrder = 2000;
}

void TinyMario::InitResource()
{
    if (animations.size() < 1 || resourceChange) {
        if (data->Power == MEntityType::SmallMario) {
            animations["Default"] = AnimationManager::GetInstance()->Get("ani-small-mario-map")->Clone();
        }
        if (data->Power == MEntityType::BigMario) {
            animations["Default"] = AnimationManager::GetInstance()->Get("ani-big-mario-map")->Clone();
        }
        if (data->Power == MEntityType::RaccoonMario) {
            animations["Default"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-map")->Clone();
        }
        if (data->Power == MEntityType::FireMario) {
            animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-mario-map")->Clone();
        }
    }
}

void TinyMario::Update()
{
    if (currentPower != data->Power) {
        currentPower = data->Power;
        resourceChange = true;
    }
    else {
        resourceChange = false;
    }

    if (moveStep == 1) {
        Vec2 distance = Position - targetPosition;
        Vec2 direction = Vec2Utils::Normalize(distance);

        Position -= direction * 0.5 * CGame::Time().ElapsedGameTime;

        if (abs(distance.x) < 3 && abs(distance.y) < 3) {
            Position = targetPosition;
            currentStation = targetStation;
            moveStep = 0;
        }

    }
}

void TinyMario::Render()
{
    InitResource();
    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
    animations["Default"]->GetTransform()->Position = Position - cam + size / 2;
    animations["Default"]->Render();
}

RectF TinyMario::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

shared_ptr<Graph> TinyMario::GetGraph()
{
    return graph;
}

void TinyMario::AddNode(shared_ptr<MapGate> node)
{
    graph->AddNode(node);
    if (data->Node < 0 && node->IsStart()) {
        data->Node = currentStation = node->GetNodeId();
        Position = node->GetPosition();
    }
}

shared_ptr<PlayerData> TinyMario::GetPlayerData()
{
    return data;
}

void TinyMario::HookEvent()
{
    __hook(&Events::KeyDownEvent, Events::GetInstance(), &TinyMario::OnKeyDown);
    __hook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneFinish);
}

void TinyMario::UnhookEvent()
{
    __unhook(&Events::KeyDownEvent, Events::GetInstance(), &TinyMario::OnKeyDown);
    __unhook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneFinish);
}

void TinyMario::OnKeyDown(int key)
{
    DebugOut(L"Hooking\n");
    Direction direction = Direction::None;
    switch (key)
    {
    case DIK_UP:
        direction = Direction::Top;
        break;
    case DIK_DOWN:
        direction = Direction::Bottom;
        break;
    case DIK_LEFT:
        direction = Direction::Left;
        break;
    case DIK_RIGHT:
        direction = Direction::Right;
        break;
    case DIK_W:
        if (moveStep == 0) {
            graph->GetNode(currentStation)->Discover();
        }
        break;
    }

    if (direction != Direction::None && moveStep == 0) {
        shared_ptr<MapGate> current = graph->GetNode(currentStation);
        unordered_map<Direction, int> adjacentList = current->GetAdjacentList();

        if (adjacentList.find(direction) == adjacentList.end()) return;

        shared_ptr<MapGate> next = graph->GetNode(adjacentList.at(direction));

        if (next->CanTravel(current, data)) {
            moveStep = 1;
            targetPosition = next->GetPosition();
            targetStation = next->GetNodeId();
        }
    }
}

void TinyMario::OnPlaySceneFinish(const char* source, CardType reward)
{
}
