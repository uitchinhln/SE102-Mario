#include "WorldScene.h"
#include "Events.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"
#include "TreeFX.h"

void WorldScene::Load()
{
	HookEvent();
	tinyMario->HookEvent();
}

void WorldScene::Unload()
{
	UnhookEvent();
	tinyMario->UnhookEvent();
}

void WorldScene::Update()
{
	camera->Update();
	hud->Update();
	tinyMario->Update();
}

void WorldScene::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));
	CGame::GetInstance()->GetGraphic().SetViewport(camera);

	gameMap->Render();

	backgroundAnimations.Render();

	//Object render
	vector<shared_ptr<GameObject>> renders;
	renders.push_back(tinyMario);
	renders.insert(renders.end(), objectsWithoutGrid.begin(), objectsWithoutGrid.end());

	sort(renders.begin(), renders.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
		});

	for each (shared_ptr<GameObject> var in renders)
	{
		if (var->IsActive() && var->Visible) var->Render();
	}

	EffectServer::GetInstance()->Render();

	CGame::GetInstance()->GetGraphic().SetViewport(hud);

	hud->Render();
}

void WorldScene::SetSceneContentPath(string path)
{
	this->dataPath = path;
	TiXmlDocument doc(this->dataPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(this->dataPath));
		return;
	}

	HookEvent();

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* cameraConfig = root->FirstChildElement("Camera");

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");
	string hudPath = root->FirstChildElement("Hud")->Attribute("path");

	this->tinyMario = make_shared<TinyMario>();

	this->data = tinyMario->GetPlayerData();
	this->data->RemainingTime = 0;

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(tinyMario);

	this->hud = make_shared<Hud>(hudPath, hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

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
	UnhookEvent();
}

void WorldScene::OnObjectLoad(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	if (strcmp(type, MEntityType::Tree.ToString().c_str()) == 0) {
		backgroundAnimations.SpawnEffect(make_shared<TreeFX>(fixedPos));
		return;
	}
	if (strcmp(type, MEntityType::Node.ToString().c_str()) == 0) {
		shared_ptr<MapGate> gate = MapGate::CreateMapGate(fixedPos, props);
		this->objectsWithoutGrid.push_back(gate);
		this->tinyMario->AddNode(gate);
		return;
	}
}

void WorldScene::OnKeyDown(int key)
{
}

void WorldScene::OnKeyUp(int key)
{
}

void WorldScene::HookEvent()
{
	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &WorldScene::OnObjectLoad);

}

void WorldScene::UnhookEvent()
{
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &WorldScene::OnObjectLoad);
}
