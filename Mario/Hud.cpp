#include "Hud.h"
#include "Game.h"
#include "Text.h"
#include "Mario.h"
#include "SceneManager.h"
#include "HudElement.h"
#include "MarioGame.h"


Hud::Hud(string tmxPath, Vec2 pos, Vec2 size) : Viewport(pos, size)
{
	coin = new Text();
	coin->SetFont(CGame::GetInstance()->DefaultFont);

	timer = new Text();
	timer->SetFont(CGame::GetInstance()->DefaultFont);

	world = new Text();
	world->SetFont(CGame::GetInstance()->DefaultFont);

	live = new Text();
	live->SetFont(CGame::GetInstance()->DefaultFont);

	score = new Text();
	score->SetFont(CGame::GetInstance()->DefaultFont);

	pmeter = new PMeter();

	this->LoadFromTmx(tmxPath);
}

void Hud::Update()
{
	shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

	panel.Update();
	cards.Update();

	pmeter->Update();

	long time = max(0, data->RemainingTime / 1000);
	string txtTime = to_string(time);
	if (txtTime.length() < 3) {
		txtTime.insert(0, 3 - txtTime.size(), '0');
	}
	timer->SetContent(txtTime);

	coin->SetContent(to_string(max(0, data->Coins)));

	world->SetContent(to_string(max(1, data->World)));

	live->SetContent(to_string(max(0, data->Lives)));

	string txtScore = to_string(max(0, data->Score));
	if (txtScore.length() < 7) {
		txtScore.insert(0, 7 - txtScore.size(), '0');
	}
	score->SetContent(txtScore);
}

void Hud::Render(D3DXCOLOR overlay)
{
	//CGame::GetInstance()->GetGraphic().Clear(background);

	panel.Render(overlay);
	cards.Render(overlay);

	pmeter->Render(overlay);

	coin->Render(overlay);
	timer->Render(overlay);
	world->Render(overlay);
	live->Render(overlay);
	score->Render(overlay);
}


void Hud::LoadFromTmx(string tmxPath)
{
	TiXmlDocument doc(tmxPath.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Cannot load hud config file.\n");
		return;
	}

	TiXmlElement* root = doc.RootElement();

	if (root->Attribute("backgroundcolor")) {
		string hexColor = root->Attribute("backgroundcolor");
		hexColor.replace(0, 1, "");
		unsigned int hex = stoul(hexColor, nullptr, 16);
		int a = (hex >> 24) & 255 | 255 & (hexColor.length() <= 6 ? 0xff : 0x00);
		background = D3DXCOLOR((hex >> 16) & 255, (hex >> 8) & 255, hex & 255, a);
	}

	for (TiXmlElement* group = root->FirstChildElement("objectgroup"); group != nullptr; group = group->NextSiblingElement("objectgroup"))
	{
		for (TiXmlElement* node = group->FirstChildElement("object"); node != nullptr; node = node->NextSiblingElement("object"))
		{
			Vec2 fixPos;
			Vec2 size = VECTOR_0;
			node->QueryFloatAttribute("x", &fixPos.x);
			node->QueryFloatAttribute("y", &fixPos.y);
			node->QueryFloatAttribute("width", &size.x);
			node->QueryFloatAttribute("height", &size.y);

			if (!node->Attribute("type")) continue;

			if (strcmp(node->Attribute("type"), HudElement::HudPanel.ToString().c_str()) == 0) {
				panel.Position = fixPos;
				panel.Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::CardSlot.ToString().c_str()) == 0) {
				cards.Add(fixPos, size);
				cards.Init();
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::Wallet.ToString().c_str()) == 0) {
				coin->Position = fixPos;
				coin->Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::RemainingTime.ToString().c_str()) == 0) {
				timer->Position = fixPos;
				timer->Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::WorldLabel.ToString().c_str()) == 0) {
				world->Position = fixPos;
				world->Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::Lives.ToString().c_str()) == 0) {
				live->Position = fixPos;
				live->Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::PMeter.ToString().c_str()) == 0) {
				pmeter->Position = fixPos;
				pmeter->Size = size;
				continue;
			}
			if (strcmp(node->Attribute("type"), HudElement::Score.ToString().c_str()) == 0) {
				score->Position = fixPos;
				score->Size = size;
				continue;
			}
		}
	}
}
