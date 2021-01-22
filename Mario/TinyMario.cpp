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
#include "MarioGame.h"

TinyMario::TinyMario()
{
    graph = make_shared<Graph>();
    Position = Vec2(0, 0);
    size = Vec2(48, 48);
    renderOrder = 2000;
}

void TinyMario::InitResource()
{
    if (animations.size() < 1 || resourceChange) {
        shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

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
    shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

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

        Position -= direction * 0.3 * CGame::Time().ElapsedGameTime;

		RectF targetHitBox = RectF(targetPosition.x + 23, targetPosition.y + 23, targetPosition.x + 25, targetPosition.y + 25);
		RectF myHitBox = RectF(Position.x + 21, Position.y + 21, Position.x + 27, Position.y + 27);

        if (CollisionCalculator::CommonAABB(myHitBox, targetHitBox)) {
            Position = targetPosition;
            currentStation = targetStation;
            moveStep = 0;
        }

    }
}

void TinyMario::Render(D3DCOLOR overlay)
{
    InitResource();
    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
    animations["Default"]->GetTransform()->Position = Position - cam + size / 2;
    animations["Default"]->Render(overlay);
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
    shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

    graph->AddNode(node);
    if (data->Node < 0 && node->IsStart()) {
        data->Node = currentStation = node->GetNodeId();
        Position = node->GetPosition();
    }
}

void TinyMario::HookEvent()
{
    //__hook(&Events::KeyDownEvent, Events::GetInstance(), &TinyMario::OnKeyDown);
    //__hook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneFinish);
    __hook(&GameEvent::PlaySceneEndEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneEnd);
    __hook(&GameEvent::PlaySceneBeginEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneBegin);
}

void TinyMario::UnhookEvent()
{
    //__unhook(&Events::KeyDownEvent, Events::GetInstance(), &TinyMario::OnKeyDown);
    //__unhook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneFinish);
    //__unhook(&GameEvent::PlaySceneLoseEvent, GameEvent::GetInstance(), &TinyMario::OnPlaySceneLose);
}

void TinyMario::OnKeyDown(int key)
{
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
    case DIK_S:
        if (moveStep == 0) {
            graph->GetNode(currentStation)->Discover();
        }
        break;
    }

    if (direction != Direction::None && moveStep == 0) {
        shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

        shared_ptr<MapGate> current = graph->GetNode(currentStation);
        unordered_map<Direction, int> adjacentList = current->GetAdjacentList();

        if (adjacentList.find(direction) == adjacentList.end()) return;

        if (current->CanTravel(direction, data)) {
            moveStep = 1;
            shared_ptr<MapGate> next = graph->GetNode(adjacentList.at(direction));
            targetPosition = next->GetPosition();
            targetStation = next->GetNodeId();
        }
    }
}

void TinyMario::OnKeyUp(int key)
{
}

void TinyMario::OnPlaySceneBegin(const char* file, int nodeId, const char* sceneId, int worldId)
{
    discoverStation = nodeId;
}

void TinyMario::OnPlaySceneEnd(const char* file, SceneResult result, CardType reward)
{
    shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();
    if (result == SceneResult::WIN) {
        data->Node = discoverStation;
        graph->GetNode(discoverStation)->SetFinished(true);
    }
    else {
        moveStep = 1;
        targetPosition = graph->GetNode(data->Node)->GetPosition();
        targetStation = data->Node;
        data->World = graph->GetNode(data->Node)->GetWorldNumber();
    }
}
