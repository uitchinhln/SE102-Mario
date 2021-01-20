#include "PlaySceneFinishFX.h"
#include "Game.h"
#include "SpriteManager.h"
#include "PlayerData.h"

PlaySceneFinishFX::PlaySceneFinishFX(Vec2 position, CardType card) : IEffect(position, 5000)
{
	line1.SetFont(CGame::GetInstance()->DefaultFont);
	line1.Position = Vec2(190, 80);
	line1.Size = Vec2(400, 24);
	line1.SetAlignment(TextAlignment::Left);
	line1.SetSpacing(3);
	line1.SetContent("COURSE CLEAR !");

	line2.SetFont(CGame::GetInstance()->DefaultFont);
	line2.Position = Vec2(133, 160);
	line2.Size = Vec2(402, 24);
	line2.SetAlignment(TextAlignment::Left);
	line2.SetSpacing(3);
	line2.SetContent("YOU GOT A CARD");

	switch (card)
	{
	case CardType::Empty:
		this->card = SpriteManager::GetInstance()->Get("spr-empty-card-0");
		break;
	case CardType::Mushroom:
		this->card = SpriteManager::GetInstance()->Get("spr-super-mushroom-card-0");
		break;
	case CardType::Flower:
		this->card = SpriteManager::GetInstance()->Get("spr-fire-flower-card-0");
		break;
	case CardType::Star:
		this->card = SpriteManager::GetInstance()->Get("spr-star-man-card-0");
		break;
	}
}

void PlaySceneFinishFX::Update()
{
	playtimeLeft -= CGame::Time().ElapsedGameTime;
	line2Delay -= CGame::Time().ElapsedGameTime;
	if (playtimeLeft < 0) Active = false;
}

void PlaySceneFinishFX::Render()
{
	line1.Render();

	if (line2Delay < 0) {
		line2.Render();
		card->Draw(540 + card->width / 2, 140 + card->height / 2, trans);
	}
}
