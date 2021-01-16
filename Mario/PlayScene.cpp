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
	TiXmlElement* cameraConfig = root->FirstChildElement("Camera");

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");

	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
	this->mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	this->mario->SetPowerUp(make_shared<Small>(mario));

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	float left = 0, top = 0, bottom = 0, right = 0;
	int startId = 0, id = 0; 

	cameraConfig->QueryIntAttribute("start", &startId);
	for (TiXmlElement* node = cameraConfig->FirstChildElement("Boundary"); node != nullptr; node = node->NextSiblingElement("Boundary"))
	{
		node->QueryIntAttribute("id", &id);
		node->QueryFloatAttribute("left", &left);
		node->QueryFloatAttribute("top", &top);
		node->QueryFloatAttribute("right", &right);
		node->QueryFloatAttribute("bottom", &bottom);
		camera->AddBound(id, left, top, right, bottom);
	}
	camera->SetActiveBound(startId);

	this->hud = make_shared<Hud>(hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	doc.Clear();
}

void PlayScene::Unload()
{
	UnhookEvent();
	objectList.clear();
	if (grid)
		grid->ClearAll();
	objects.clear();
	objectsWithoutGrid.clear();
	camera.reset();
	hud.reset();
	gameMap.reset();
}

void PlayScene::Update()
{
	//auto start = std::chrono::high_resolution_clock::now();	
	RectF camBox = camera->GetBoundingBox();

	objectList.clear();
	grid->GetByCamera(this->camera, this->objects, objectList);
	objectList.insert(objectList.end(), objectsWithoutGrid.begin(), objectsWithoutGrid.end());

	gameMap->Update();

	mario->Update();

	for (shared_ptr<GameObject> obj : objectList) {
		obj->Update();
	}

	vector<shared_ptr<GameObject>> objs;
	objs.clear();
	objs.insert(objs.end(), objectList.begin(), objectList.end());
	mario->CollisionUpdate(&objs);	

	#pragma region CollisionUpdate
	objs.push_back(this->mario);
	for (shared_ptr<GameObject> obj : objectList) {
		obj->CollisionUpdate(&objs);
	}

	mario->CollisionDoubleFilter();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->CollisionDoubleFilter();
	}

	mario->PositionUpdate();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->PositionUpdate();
	}

	mario->RestoreCollision();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->RestoreCollision();
	}

	mario->PositionLateUpdate();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->PositionLateUpdate();
	}
	#pragma endregion

	mario->StatusUpdate();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->StatusUpdate();
	}

	mario->FinalUpdate();
	for (shared_ptr<GameObject> obj : objectList) {
		obj->FinalUpdate();

		RectF objBox = obj->GetHitBox();
		if (!CollisionCalculator::CommonAABB(camBox, objBox)) {
			if (obj->GetInCamera) obj->OnLostCamera();
			obj->GetInCamera = false;
		}
		else {
			if (!obj->GetInCamera) obj->OnGetInCamera();
			obj->GetInCamera = true;
		}
	}
	
	RemoveDespawnedObjects();

	camera->Update();
	hud->Update();
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Update: static: %d\tmoving: %d\t%d\n", objectList.size(), objectList.size(), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

void PlayScene::Render()
{
	//auto start = std::chrono::high_resolution_clock::now();
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));	
	CGame::GetInstance()->GetGraphic().SetViewport(camera);

	gameMap->Render();
	vector<shared_ptr<GameObject>> renderObjects;
	renderObjects.insert(renderObjects.end(), objectList.begin(), objectList.end());
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
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Render: static: %d\tmoving: %d\t%d\n", objectList.size(), objectList.size(), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

void PlayScene::SpawnEntity(shared_ptr<GameObject> entity)
{
	CScene::SpawnEntity(entity);
	grid->Add(entity);
}

void PlayScene::SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props)
{
	CScene::SpawnEntity(entity, props);
	if (props.HasProperty("Grid")) {
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

vector<shared_ptr<GameObject>> PlayScene::GetUpdatingObjects()
{
	return objectList;
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
	//Player Spawn Point
	if (strcmp(type, "SpawnPoint") == 0) {
		RectF marioBox = mario->GetHitBox();
		mario->SetPosition(fixedPos - Vec2(0, marioBox.bottom - marioBox.top));
	}

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

	grid = new Grid(totalCol, totalRow, cellW, cellH);
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
