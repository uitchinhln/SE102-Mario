#include "PowerMeter.h"
#include "SpriteManager.h"
#include "Game.h"

PowerMeter::PowerMeter()
{
	arrowOn = SpriteManager::GetInstance()->Get("spr-arrow-0");
	arrowOff = SpriteManager::GetInstance()->Get("spr-arrow-1");
	powerOn = SpriteManager::GetInstance()->Get("spr-p-icon-0");
	powerOff = SpriteManager::GetInstance()->Get("spr-p-icon-1");
	spacing = -3;

	blink = 0;
}

void PowerMeter::Update()
{
	if (this->level == 7) blink += CGame::Time().ElapsedGameTime;
	if (blink > BLINK_TIME) blink = 0;
}

void PowerMeter::Render()
{
	Transform trans;
	if (arrowOn == nullptr) return;
	if (arrowOff == nullptr) return;
	if (powerOn == nullptr) return;
	if (powerOff == nullptr) return;

	// Draw arrow
	int x = 0;
	int w = arrowOn->width;
	for (int i = 1; i <= 6; i++)
	{
		arrowOff->Draw(Position.x + x, Position.y, trans);
		x += w + spacing;
	}
	powerOff->Draw(Position.x + x, Position.y, trans);

	x = 0;
	w = arrowOn->width;
	for (int i = 1; i <= min(this->level, 6); i++)
	{
		arrowOn->Draw(Position.x + x, Position.y, trans);
		x += w + spacing;
	}
	if (this->level == 7 && blink > BLINK_TIME * 0.5f)
		powerOn->Draw(Position.x + x, Position.y, trans);
}

void PowerMeter::SetLevel(int level)
{
	this->level = max(0, min(level, 7));
}
