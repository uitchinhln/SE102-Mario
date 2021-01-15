#include "PlayScene.h"
#include "Game.h"
#include "MarioPowerUp.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "SceneManager.h"
#include "RaccoonMario.h"
#include "Events.h"

#include "Goomba.h"
#include "Koopas.h"

#include "EndmapReward.h"
#include "QuestionBlock.h"

#include "SolidBlock.h"
#include "GhostBlock.h"
#include "Pipe.h"
#include "Spawner.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "RedGoomba.h"
#include "Coin.h"
#include "VoidBlock.h"

#include "rapidjson/document.h"

using namespace rapidjson;

void PlayScene::Load()
{
	HookEvent();

	TiXmlDocument doc(this->dataPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(this->dataPath));
		return;
	}

	TiXmlElement* root = doc.RootElement();

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");

	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
	this->mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	this->mario->SetPowerUp(make_shared<Small>(mario));

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	this->hud = make_shared<Hud>(hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	doc.Clear();
}

void PlayScene::Unload()
{
	UnhookEvent();
	staticObjects.clear();
	movingObjects.clear();
	if (staticObjectGrid) 
		staticObjectGrid->ClearAll();
	if (movingObjectGrid)
		movingObjectGrid->ClearAll();
	objects.clear();
	camera.reset();
	hud.reset();
	gameMap.reset();
}

void PlayScene::Update()
{
	staticObjects.clear();
	movingObjects.clear();

	auto start = std::chrono::high_resolution_clock::now();
	staticObjectGrid->GetByCamera(this->camera, this->objects, staticObjects);
	movingObjectGrid->GetByCamera(this->camera, this->objects, movingObjects);
	auto finish = std::chrono::high_resolution_clock::now();
	DebugOut(L"Loop: static: %d\tmoving: %d\t%d\n", staticObjects.size(), movingObjects.size(), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());

	gameMap->Update();

	mario->Update();

	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->Update();
	}

	vector<shared_ptr<GameObject>> objs;
	objs.clear();
	objs.insert(objs.end(), movingObjects.begin(), movingObjects.end());
	objs.insert(objs.end(), staticObjects.begin(), staticObjects.end());
	mario->CollisionUpdate(&objs);	

#pragma region CollisionUpdate
	objs.push_back(this->mario);
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->CollisionUpdate(&objs);
	}

	mario->CollisionDoubleFilter();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->CollisionDoubleFilter();
	}

	mario->PositionUpdate();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->PositionUpdate();
	}

	mario->RestoreCollision();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->RestoreCollision();
	}

	mario->PositionLateUpdate();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->PositionLateUpdate();
	}
#pragma endregion

	mario->StatusUpdate();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->StatusUpdate();
	}

	mario->FinalUpdate();
	for (shared_ptr<GameObject> obj : movingObjects) {
		obj->FinalUpdate();
	}

	RemoveDespawnedObjects();

	camera->Update();
	hud->Update();
}

void PlayScene::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));	
	CGame::GetInstance()->GetGraphic().SetViewport(camera);

	gameMap->Render();
	vector<shared_ptr<GameObject>> renderObjects;
	renderObjects.insert(renderObjects.end(), movingObjects.begin(), movingObjects.end());
	renderObjects.push_back(mario);

	sort(renderObjects.begin(), renderObjects.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
	});

	for each (shared_ptr<GameObject> obj in renderObjects)
	{
		if (obj->Visible && obj->IsActive()) obj->Render();
	}

	CGame::GetInstance()->GetGraphic().SetViewport(hud);

	hud->Render();
}

void PlayScene::SpawnEntity(shared_ptr<GameObject> entity)
{
	CScene::SpawnEntity(entity);
}

void PlayScene::SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props)
{
	CScene::SpawnEntity(entity, props);
	if (props.HasProperty("Grid")) {
		Grid* grid = MEntityType::IsMapObject(entity->GetObjectType()) ? staticObjectGrid : movingObjectGrid;
		Document document;
		document.Parse(props.GetText("Grid").c_str());
		for (auto& v : document.GetArray()) {
			auto cr = v.GetObjectW();
			int x = cr["x"].GetInt();
			int y = cr["y"].GetInt();

			grid->Add(entity->GetID(), x, y);
		}
		document.Clear();
	}
}

vector<shared_ptr<GameObject>> PlayScene::GetMovingObjects()
{
	return movingObjects;
}

void PlayScene::OnKeyDown(int key)
{
	if (!mario) return;
	mario->OnKeyDown(key);
}

void PlayScene::OnKeyUp(int key)
{
	if (!mario) return;
	mario->OnKeyUp(key);
}

void PlayScene::ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	//GameObjects
	if (strcmp(type, MEntityType::Goomba.ToString().c_str()) == 0) {
		SpawnEntity(Goomba::CreateGoomba(fixedPos), props);
	}
	if (strcmp(type, MEntityType::RedGoomba.ToString().c_str()) == 0) {
		SpawnEntity(RedGoomba::CreateRedGoomba(fixedPos), props);
	}
	if (strcmp(type, MEntityType::Koopas.ToString().c_str()) == 0) {
		SpawnEntity(Koopas::CreateKoopas(fixedPos), props);
	}
	if (strcmp(type, MEntityType::KoopasJumping.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<JumpingKoopas>(kp));
		SpawnEntity(kp, props);
	}
	if (strcmp(type, MEntityType::RedKoopas.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<DefRedKoopas>(kp));
		SpawnEntity(kp, props);
	}
	if (strcmp(type, MEntityType::Venus.ToString().c_str()) == 0) {
		SpawnEntity(Venus::CreateVenus(fixedPos), props);
	}
	if (strcmp(type, MEntityType::RedVenus.ToString().c_str()) == 0) {
		SpawnEntity(RedVenus::CreateRedVenus(fixedPos), props);
	}
	if (strcmp(type, MEntityType::Piranha.ToString().c_str()) == 0) {
		SpawnEntity(Piranha::CreatePiranha(fixedPos), props);
	}
	if (strcmp(type, MEntityType::EndmapReward.ToString().c_str()) == 0) {
		SpawnEntity(EndmapReward::CreateEndmapReward(fixedPos), props);
	}
	if (strcmp(type, MEntityType::QuestionBlock.ToString().c_str()) == 0) {
		SpawnEntity(QuestionBlock::CreateQuestionBlock(fixedPos, props), props);
	}
	if (strcmp(type, MEntityType::Spawner.ToString().c_str()) == 0) {
		SpawnEntity(Spawner::CreateSpawner(fixedPos, props), props);
	}
	if (strcmp(type, MEntityType::Pipe.ToString().c_str()) == 0) {
		SpawnEntity(Pipe::CreatePipe(fixedPos, size, props), props);
	}
	if (strcmp(type, MEntityType::Coin.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::COIN), props);
	}
	if (strcmp(type, MEntityType::Brick.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::BRICK), props);
	}

	//MapObjects
	if (strcmp(type, MEntityType::SolidBlock.ToString().c_str()) == 0) {
		SpawnEntity(SolidBlock::CreateSolidBlock(fixedPos, size), props);
	}
	if (strcmp(type, MEntityType::GhostBlock.ToString().c_str()) == 0) {
		SpawnEntity(GhostBlock::CreateGhostBlock(fixedPos, size), props);
	}
	if (strcmp(type, MEntityType::VoidBlock.ToString().c_str()) == 0) {
		SpawnEntity(VoidBlock::CreateVoidBlock(fixedPos, size), props);
	}
}

void PlayScene::MapReadEvent(MapProperties& props)
{
	int cellW = props.GetInt("CellWidth");
	int cellH = props.GetInt("CellHeight");
	int totalCol = props.GetInt("TotalColumn");
	int totalRow = props.GetInt("TotalRow");

	movingObjectGrid = new Grid(totalCol, totalRow, cellW, cellH);
	staticObjectGrid = new Grid(totalCol, totalRow, cellW, cellH);
}

PlayScene::~PlayScene()
{
	Unload();
}

void PlayScene::HookEvent()
{
	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
	__hook(&Events::MapReadEvent, Events::GetInstance(), &PlayScene::MapReadEvent);
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
	__unhook(&Events::MapReadEvent, Events::GetInstance(), &PlayScene::MapReadEvent);
}
