#include "Hud.h"
#include "Game.h"
#include "Text.h"
#include "Mario.h"
#include "SceneManager.h"

Hud::Hud(Vec2 pos, Vec2 size) : Viewport(pos, size)
{
	auto sprManager = SpriteManager::GetInstance();
	panel = sprManager->Get("spr-hud-0");

	cards.push_back(ItemCard{ ItemCardType::Empty });
	cards.push_back(ItemCard{ ItemCardType::Empty });
	cards.push_back(ItemCard{ ItemCardType::Empty });

	cardVs[(int)ItemCardType::Empty] = sprManager->Get("spr-empty-card-0");
	cardVs[(int)ItemCardType::Mushroom] = sprManager->Get("spr-super-mushroom-card-0");
	cardVs[(int)ItemCardType::Fireflower] = sprManager->Get("spr-fire-flower-card-0");
	cardVs[(int)ItemCardType::Superstar] = sprManager->Get("spr-star-man-card-0");

	world = new Text();
	life = new Text();
	score = new Text();
	coin = new Text();
	timer = new Text();

	auto font = CGame::GetInstance()->DefaultFont;
	world->SetFont(font);
	life->SetFont(font);
	score->SetFont(font);
	coin->SetFont(font);
	timer->SetFont(font);

	powerMeter = new PowerMeter();


	world->SetContent("1");
	life->SetContent("4");
	score->SetContent("0002340");
	coin->SetContent("11");
	timer->SetContent("000");

	// Organize layout
	world->Position = Vec2(110, 24);
	life->Position = Vec2(110 - 4, 48);
	score->Position = Vec2(150, 48);

	coin->SetAlignment(TextAlignment::Right);
	coin->Position = Vec2(448, 24);
	timer->SetAlignment(TextAlignment::Right);
	timer->Position = Vec2(448, 48);

	powerMeter->Position = Vec2(150, 20);
}

void Hud::Update()
{
	if (!player) {
		player = SceneManager::GetInstance()->GetPlayer<Mario>();
	}

	powerMeter->SetLevel(player->GetPowerMeter());
	powerMeter->Update();
}

void Hud::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	Transform trans;
	panel->Draw(0, 0, trans);

	//for (int i = cards.size() - 1; i >= 0; --i)
	//{
	//	int x = config.screenWidth - cardVisuals[i]->GetSpriteWidth() * (cards.size() - i);
	//	int y = config.screenHeight - config.hudOffset;
	//	DrawCard(cards[i], x - 32, y);
	//}

	world->Render();
	life->Render();
	score->Render();
	coin->Render();
	timer->Render();

	powerMeter->Render();
}

void Hud::DrawCard(ItemCard& card, int x, int y)
{
}
