#include "IntroScene.h"
#include "MSceneType.h"
#include "MarioGame.h"
#include "Mario.h"
#include "Events.h"
#include "EffectServer.h"
#include "IntroController.h"

IntroScene::IntroScene()
{
	backgroundEffect = new EffectServer();
	IntroController::GetInstance()->SetScene(this);
}

void IntroScene::Load()
{
	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &IntroScene::ObjectLoadEvent);

	TiXmlDocument doc(this->dataPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(this->dataPath));
		return;
	}

	TiXmlElement* root = doc.RootElement();

	string mapPath = root->FirstChildElement("TmxMap")->Attribute("path");

	this->camera = make_shared<Camera>(camSize);
	this->camera->LoadFromTMX(root->FirstChildElement("Camera"));
	this->camera->SetTracking(MarioGame::GetInstance()->GetMario());

	this->gameMap = CGameMap::FromTMX(mapPath);
	this->gameMap->SetCamera(camera);

	doc.Clear();

	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &IntroScene::ObjectLoadEvent);
}

void IntroScene::Unload()
{
	this->objectList.clear();
	this->objects.clear();
	this->objectsWithoutGrid.clear();

	this->camera.reset();
	this->gameMap.reset();

	backgroundEffect->Clear();
	EffectServer::GetInstance()->Clear();
}

void IntroScene::Update()
{
	IntroController::GetInstance()->Update();
	OriginalUpdate();
	backgroundEffect->Update();
	EffectServer::GetInstance()->Update();
}

void IntroScene::SetBackgroundColor(D3DCOLOR color)
{
	gameMap->SetBackgroundColor(color);
}

void IntroScene::Render(D3DCOLOR overlay)
{
	backgroundEffect->Render(overlay);
	OriginalRender(overlay);
	EffectServer::GetInstance()->Render(overlay);
}

EffectServer* IntroScene::GetBackgroundEffect()
{
	return backgroundEffect;
}

vector<shared_ptr<GameObject>> IntroScene::GetUpdatingObjects()
{
    return vector<shared_ptr<GameObject>>();
}

ObjectType IntroScene::GetSceneType()
{
    return MSceneType::IntroScene;
}

