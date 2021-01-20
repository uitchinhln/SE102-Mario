#include "MapGate.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "PlayerData.h"

MapGate::MapGate(int nodeId, string sceneID, string uncheck, string checked)
{
	this->nodeId = nodeId;
	this->sceneID = sceneID;
	this->renderOrder = 1000;

	if (!uncheck.empty() && !checked.empty()) {
		this->sprites["Uncheck"] = SpriteManager::GetInstance()->Get(uncheck);
		this->sprites["Checked"] = SpriteManager::GetInstance()->Get(checked);
	}
}

void MapGate::InitResource()
{
	if (sprites.size() < 1) {
		
	}
}

void MapGate::Update()
{
}

void MapGate::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void MapGate::PositionUpdate()
{
}

void MapGate::RestoreCollision()
{
}

void MapGate::PositionLateUpdate()
{
}

void MapGate::CollisionDoubleFilter()
{
}

void MapGate::StatusUpdate()
{
}

void MapGate::FinalUpdate()
{
}

bool MapGate::HasCollideWith(DWORD id)
{
	return false;
}

void MapGate::Render()
{
	if (sprites.size() == 2) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
		if (!finish) {
			this->sprites["Uncheck"]->Draw(Position.x - cam.x + size.x / 2, Position.y - cam.y + size.y / 2, trans);
		}
		else {
			this->sprites["Checked"]->Draw(Position.x - cam.x + size.x / 2, Position.y - cam.y + size.y / 2, trans);
		}
	}
}

bool MapGate::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float MapGate::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

ObjectType MapGate::GetObjectType()
{
	return MEntityType::Node;
}

bool MapGate::CanTravel(shared_ptr<MapGate> currentStation, shared_ptr<PlayerData> data)
{
	return true;
}

void MapGate::Discover()
{
	if (sceneID.empty() || finish) return;
	SceneManager::GetInstance()->ActiveScene(sceneID);
}

bool MapGate::IsFinished()
{
	return finish;
}

void MapGate::SetFinished(bool value)
{
	this->finish = value;
}

int MapGate::GetWorldNumber()
{
	return this->worldNumber;
}

bool MapGate::IsStart()
{
	return start;
}

int MapGate::GetNodeId()
{
	return nodeId;
}

string MapGate::GetSceneID()
{
	return sceneID;
}

unordered_map<Direction, int>& MapGate::GetAdjacentList()
{
	return adjacentNodes;
}

shared_ptr<MapGate> MapGate::CreateMapGate(Vec2 pos, MapProperties props)
{
	string adjacent_list = props.GetText("adjacent_list", "");
	string adjacent_weight = props.GetText("adjacent_weight", "");

	int nodeId = props.GetInt("node_id");

	string scene = props.GetText("scene");

	string sprite_checked = props.GetText("sprite_checked");
	string sprite_uncheck = props.GetText("sprite_uncheck");

	shared_ptr<MapGate> gate = make_shared<MapGate>(nodeId, scene, sprite_uncheck, sprite_checked);
	gate->size = Vec2(48, 48);
	gate->Position = pos;
	gate->start = props.GetBool("Start", false);
	gate->worldNumber = props.GetInt("world_number", 0);

	vector<string> adjacents = split(adjacent_list, ",");
	for (int i = 0; i < adjacents.size(); i++)
	{
		int nodeID = stoi(adjacents[i]);
		Direction direction = Direction::None;
		switch (adjacent_weight[2*i])
		{
		case 'l':
			direction = Direction::Left;
			break;
		case 'u':
			direction = Direction::Top;
			break;
		case 'r':
			direction = Direction::Right;
			break;
		case 'd':
			direction = Direction::Bottom;
			break;
		}
		
		if (direction != Direction::None) {
			gate->adjacentNodes[direction] = nodeID;
		}
	}

	return gate;
}
