#include "FadeFX.h"
#include "TextureManager.h"
#include "MarioGame.h"

FadeFX::FadeFX(RECT screenRect, long playTime, function<void(long)> callback)
	: ICallbackEffect(Vec2(screenRect.left, screenRect.top), playTime, callback)
{
	texture = TextureManager::GetInstance()->Get("tex-box");

	this->r = RECT{ 0, 0, 48, 48};
	this->playTime = playTime;
	this->width = screenRect.right - screenRect.left;
	this->height = screenRect.bottom - screenRect.top;
	this->timer = 0;
}

void FadeFX::Update()
{
	timer += MarioGame::Time().ElapsedGameTime;

	if (timer > playTime / 2) {
		if (step == 0) {
			callback(timer);
			step = 1;
		}
	}

	if (timer > playTime) {
		Active = false;
	}
}

void FadeFX::Render(D3DXCOLOR overlay)
{
	DWORD alpha = 0;
	float range = playTime / 2;
	float played = abs(range - timer);

	if (timer <= range) {
		alpha = 255 - 255 * (played / range);
	}
	else {
		alpha = 255 - 255 * (played / range);
	}

	overlay = D3DXCOLOR(255, 255, 255, alpha);

	for (int i = 0; i < width; i += 48)
	{
		for (int j = 0; j < height; j += 48)
		{
			MarioGame::GetInstance()->GetGraphic().Draw(i + 24, j + 24, pivot, texture, r, trans, overlay);
		}
	}
}
