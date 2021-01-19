#include "PMeter.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"

PMeter::PMeter()
{
	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
}

void PMeter::InitResource()
{
	if (sprites.size() < 1) {
		sprites["ArrowOn"] = SpriteManager::GetInstance()->Get("spr-arrow-0");
		sprites["ArrowOff"] = SpriteManager::GetInstance()->Get("spr-arrow-1");
		sprites["PowerOn"] = SpriteManager::GetInstance()->Get("spr-p-icon-0");
		sprites["PowerOff"] = SpriteManager::GetInstance()->Get("spr-p-icon-1");
	}
}

void PMeter::Update()
{
	level = (int) mario->GetPowerMeter();
	level = min(7, max(0, level));
}

void PMeter::Render()
{
	InitResource();
	float begin = Position.x;
	int width = sprites["ArrowOff"]->width;

	for (int i = 1; i < 7; i++)
	{
		if (i > level) {
			sprites["ArrowOff"]->Draw(begin + width / 2, Position.y + Size.y / 2, trans);
		}
		else {
			sprites["ArrowOn"]->Draw(begin + width / 2, Position.y + Size.y / 2, trans);
		}
		begin += width;
	}

	width = sprites["PowerOn"]->width;
	if (level == 7 && (CGame::Time().TotalGameTime / 50) % 2) {
		sprites["PowerOn"]->Draw(begin + width / 2, Position.y + Size.y / 2, trans);
	}
	else {
		sprites["PowerOff"]->Draw(begin + width / 2, Position.y + Size.y / 2, trans);
	}
}
