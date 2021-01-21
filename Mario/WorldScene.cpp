#include "WorldScene.h"
#include "Events.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"
#include "TreeFX.h"
#include "MSceneType.h"
#include "MarioGame.h"

void WorldScene::Load()
{
}

void WorldScene::Unload()
{
	//UnhookEvent();
	//tinyMario->UnhookEvent();
}

void WorldScene::Update()
{
	camera->Update();
	tinyMario->Update();
}

void WorldScene::Render(D3DCOLOR overlay)
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	gameMap->Render(overlay);

	backgroundAnimations.Render(overlay);

	//Object render
	vector<shared_ptr<GameObject>> renders;
	renders.push_back(tinyMario);
	renders.insert(renders.end(), objectsWithoutGrid.begin(), objectsWithoutGrid.end());

	sort(renders.begin(), renders.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
		});

	for each (shared_ptr<GameObject> var in renders)
	{
		if (var->IsActive() && var->Visible) var->Render(overlay);
	}

	EffectServer::GetInstance()->Render(overlay);
}

void WorldScene::SetSceneContentPath(string path)
{
	this->dataPath = path;
	TiXmlDocument doc(this->dataPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(this->dataPath));
		return;
	}

	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &WorldScene::OnObjectLoad);

	TiXmlElement* root = doc.RootElement();

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");

	this->tinyMario = MarioGame::GetInstance()->GetTinyMario();

	this->camera = make_shared<Camera>(camSize);
	this->camera->LoadFromTMX(root->FirstChildElement("Camera"));
	this->camera->SetTracking(tinyMario);

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	doc.Clear();

	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &WorldScene::OnObjectLoad);
}

ObjectType WorldScene::GetSceneType()
{
	return MSceneType::Overworld;
}

void WorldScene::OnObjectLoad(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	if (strcmp(type, "SpawnPoint") == 0) {
		RectF marioBox = tinyMario->GetHitBox();
		tinyMario->SetPosition(fixedPos - Vec2(0, marioBox.bottom - marioBox.top));

		camera->Position.x = props.GetFloat("cameraX");
		camera->Position.y = props.GetFloat("cameraY");
	}
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
	if (!tinyMario) return;
	tinyMario->OnKeyDown(key);
}

void WorldScene::OnKeyUp(int key)
{
	if (!tinyMario) return;
	tinyMario->OnKeyUp(key);
}

void WorldScene::HookEvent()
{
}

void WorldScene::UnhookEvent()
{
}
