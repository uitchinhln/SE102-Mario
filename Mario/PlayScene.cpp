#include "PlayScene.h"
#include "Game.h"
#include "MarioPower.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "SceneManager.h"
#include "RaccoonMario.h"
#include "Events.h"
#include "GameEvent.h"
#include "EffectServer.h"

#include "Goomba.h"
#include "Koopas.h"

#include "EndmapReward.h"
#include "QuestionBlock.h"

#include "SolidBlock.h"
#include "GhostBlock.h"
#include "VoidBlock.h"
#include "Pipe.h"

#include "Spawner.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "RedGoomba.h"
#include "Coin.h"

#include "BeginPortal.h"
#include "EndPortal.h"

#include "rapidjson/document.h"
#include "PlaySceneFinishFX.h"

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
	string hudPath = root->FirstChildElement("Hud")->Attribute("path");

	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
	this->mario->HookEvent();

	this->data = mario->GetPlayerData();
	this->data->RemainingTime = 300000;

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	this->hud = make_shared<Hud>(hudPath, hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	Vec2 mapBound = gameMap->GetBound();
	this->mario->MovingBound = RectF(0, -mapBound.y, mapBound.x, mapBound.y);

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

	doc.Clear();
}

void PlayScene::Unload()
{
	UnhookEvent();
	this->mario->UnHookEvent();

	this->finish = false;
	this->finishEffectTimer = 0;
	this->pauseGameUpdate = false;
	this->pauseGameRender = false;
	this->objectList.clear();
	this->objects.clear();
	this->objectsWithoutGrid.clear();

	if (grid) grid->ClearAll();

	this->camera.reset();
	this->hud.reset();
	this->gameMap.reset();

	EffectServer::GetInstance()->Clear();
}

void PlayScene::Update()
{
	//auto start = std::chrono::high_resolution_clock::now();	
	
	RectF camBox = camera->GetBoundingBox();

	camera->Update();
	hud->Update();

	if (!pauseGameUpdate && !finish) {
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

		this->data->RemainingTime -= CGame::Time().ElapsedGameTime;
	}

	if (finish) {
		finishEffectTimer += CGame::Time().ElapsedGameTime;

		if (finishEffectTimer > 2000) {
			if (data->RemainingTime > 0) {
				int a = CGame::Time().ElapsedGameTime / 4;

				data->RemainingTime -= 1000 * a;
				data->Score += 50 * a;
				
				if (data->RemainingTime <= 0) {
					data->RemainingTime = 0;
					finishEffectTimer = 100000;
				};
			}
			else {
				if (finishEffectTimer > 101000) {
					mario->SetLockController(false);
					mario->SetVelocity(VECTOR_0);
					mario->GetDistance() = VECTOR_0;
					mario->ClearInhand();
					mario->Visible = true;
					mario->SetCollidibility(true);
					mario->SetActive(true);
					data->Power = mario->GetObjectType();

					if (data->Cards.size() >= 3) data->Cards.clear();

					SceneManager::GetInstance()->ActiveScene("overworld");
					return;
				}
			}
		}
	}

	EffectServer::GetInstance()->Update();

	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Update: objects: %d\t%d\n", objectList.size(), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

void PlayScene::Render()
{
	//auto start = std::chrono::high_resolution_clock::now();

	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));	
	CGame::GetInstance()->GetGraphic().SetViewport(camera);

	if (!pauseGameRender) {
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
			obj->RenderTestBox();
		}
	}

	EffectServer::GetInstance()->Render();

	CGame::GetInstance()->GetGraphic().SetViewport(hud);

	hud->Render();

	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Render: objects: %d\t%d\n", objectList.size(), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
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

void PlayScene::OnPlaySceneFinish(const char* source, CardType reward)
{
	finish = true;
	data->Cards.push_back(reward);
	EffectServer::GetInstance()->SpawnEffect(make_shared<PlaySceneFinishFX>(Vec2(100, 100), reward));
}

void PlayScene::ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	//Player Spawn Point
	if (strcmp(type, "SpawnPoint") == 0) {
		RectF marioBox = mario->GetHitBox();
		mario->SetPosition(fixedPos - Vec2(0, marioBox.bottom - marioBox.top ));
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
	if (strcmp(type, MEntityType::BeginPortal.ToString().c_str()) == 0) {
		SpawnEntity(BeginPortal::CreatePortal(fixedPos, size, props), props);
	}
	if (strcmp(type, MEntityType::EndPortal.ToString().c_str()) == 0) {
		SpawnEntity(EndPortal::CreatePortal(fixedPos, size, props), props);
	}
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

void PlayScene::SetSceneContentPath(string path)
{
	TiXmlDocument doc(path.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(path));
		return;
	}
	DebugOut(L"Get scene data success. File: %s\n", ToLPCWSTR(path));

	doc.Clear();

	this->dataPath = path;
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
	__hook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &PlayScene::OnPlaySceneFinish);
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
	__unhook(&Events::MapReadEvent, Events::GetInstance(), &PlayScene::MapReadEvent);
	__unhook(&GameEvent::PlaySceneFinishEvent, GameEvent::GetInstance(), &PlayScene::OnPlaySceneFinish);
}
