#include "PlayScene.h"
#include "MarioGame.h"
#include "SceneManager.h"
#include "Events.h"
#include "GameEvent.h"
#include "EffectServer.h"

#include "rapidjson/document.h"
#include "PlaySceneFinishFX.h"
#include "MarioGame.h"

using namespace rapidjson;

void PlayScene::Load()
{
	HookEvent();

	MarioGame::GetInstance()->GetPlayerData()->RemainingTime = 300000;

	this->mario = MarioGame::GetInstance()->GetMario();
	this->mario->HookEvent();

	TiXmlDocument doc(this->dataPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(this->dataPath));
		return;
	}

	float left = 0, top = 0, bottom = 0, right = 0;
	int startId = 0, id = 0;

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* cameraConfig = root->FirstChildElement("Camera");

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	Vec2 mapBound = gameMap->GetBound();
	this->mario->MovingBound = RectF(0, -mapBound.y, mapBound.x, mapBound.y);


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

	this->objectList.clear();
	this->objects.clear();
	this->objectsWithoutGrid.clear();

	if (grid) grid->ClearAll();

	this->camera.reset();
	this->gameMap.reset();

	EffectServer::GetInstance()->Clear();
}

void PlayScene::Update()
{	
	RectF camBox = camera->GetBoundingBox();

	camera->Update();

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

	EffectServer::GetInstance()->Update();
}

void PlayScene::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));	

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

	EffectServer::GetInstance()->Render();
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
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
	__unhook(&Events::MapReadEvent, Events::GetInstance(), &PlayScene::MapReadEvent);
}
