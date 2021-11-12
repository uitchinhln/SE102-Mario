#include "RectangleInFX.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "MarioGame.h"

RectangleInFX::RectangleInFX(RECT screenRect, long playTime, Vec2 stepSize, function<void(long)> callback)
	: ICallbackEffect(Vec2(screenRect.left, screenRect.top), playTime, callback)
{
	this->stepSize = stepSize;
	this->r = RECT{ 0, 0, (long)stepSize.x, (long)stepSize.y };
	this->playTime = playTime;
	this->playtimeLeft = 0;
	this->width = screenRect.right - screenRect.left;
	this->height = screenRect.bottom - screenRect.top;
	this->speed = playTime / max(1, min(width / (2 * stepSize.x), height / (2 * stepSize.y)));

	texture = TextureManager::GetInstance()->Get("tex-box");
}

void RectangleInFX::Update()
{
	playtimeLeft += MarioGame::Time().ElapsedGameTime;

	if (playtimeLeft > playTime && !executed) {
		callback(playtimeLeft);
		executed = true;
		//Active = false;
	}

	if (playtimeLeft > playTime + fadeOutTime) {
		Active = false;
	}

	current = min(playtimeLeft, playTime) / max(speed, 1);
}

void RectangleInFX::Render(D3DXCOLOR overlay)
{
	Vec2 back = stepSize / 2;

	if (executed) {
		float range = fadeOutTime;
		float played = abs(playtimeLeft - playTime);

		DWORD alpha = max(0, 255 - 255 * (played / range) - 50);

		for (int i = 0; i < width; i += stepSize.x)
		{
			for (int j = 0; j < height; j += stepSize.y)
			{
				CGame::GetInstance()->GetGraphic().Draw(i + back.x, j + back.y, pivot, texture, r, trans, D3DXCOLOR(alpha, 255, 255, 255));
			}
		}
	}
	else {
		for (int i = 0; i <= current; i++)
		{
			Vec2 pos = stepSize * i;

			for (int iw = 0; iw < width; iw += stepSize.x)
			{
				CGame::GetInstance()->GetGraphic().Draw(iw + back.x, pos.x + back.y,
					pivot, texture, r, trans, overlay);

				CGame::GetInstance()->GetGraphic().Draw(iw + back.x, height - pos.x + back.y,
					pivot, texture, r, trans, overlay);
			}

			for (int ih = 0; ih < height; ih += stepSize.y)
			{
				CGame::GetInstance()->GetGraphic().Draw(pos.y + back.x, ih + back.y,
					pivot, texture, r, trans, overlay);

				CGame::GetInstance()->GetGraphic().Draw(width - pos.y + back.x, ih + back.y,
					pivot, texture, r, trans, overlay);
			}
		}
	}
}
