#include "ThreeFX.h"
#include "MarioGame.h"
#include "AnimationManager.h"

ThreeFX::ThreeFX(Vec2 pos, long playTime, function<void(long)> callback) : ICallbackEffect(pos, playTime, callback)
{
	playtimeLeft = playTime;
}

void ThreeFX::Update()
{
	timer += MarioGame::Time().ElapsedGameTime;

	if (step == 0) {
		_titleP.y += 0.7 * MarioGame::Time().ElapsedGameTime;
		_verP.y += 0.7 * MarioGame::Time().ElapsedGameTime;

		if (_titleP.y > titleP.y) {
			_titleP = titleP;
			_verP = verP;
			timer = 0;
			step = 2;
		}
	}
	else if (step == 2) {
		_titleP = titleP;
		_verP = verP;

		int shake = rand() % 8 - rand() % 8;

		_titleP.y += shake;
		_verP.y += shake;

		if (timer > 1000) {
			_titleP = titleP;
			_verP = verP;
			timer = 0;
			step = 3;
		}
	}
	else if (step == 3) {
		if (timer > 1200) {
			callback(1);
		}
		if (timer > 1300) {
			callback(2);
		}
		if (timer > 1400) {
			callback(3);
		}
		if (timer > 1500) {
			callback(4);
			timer = 0;
			step = 4;
		}
	}
	else if (step == 4) {
		callback(5);
		step = 6;
	}
}

void ThreeFX::Render(D3DCOLOR overlay)
{
	InitResource();

	animations["Title"]->GetTransform()->Position = _titleP + titleS / 2;
	animations["Title"]->Render(overlay);

	animations["Version"]->GetTransform()->Position = _verP + verS / 2;
	animations["Version"]->Render(overlay);

	if (step > 3) {
		animations["BottomCurtain"]->GetTransform()->Position = curP + curS / 2;
		animations["BottomCurtain"]->Render(overlay);
	}
}

void ThreeFX::InitResource()
{
	if (animations.size() < 1) {
		animations["Title"] = AnimationManager::GetInstance()->Get("ani-title")->Clone();
		animations["Version"] = AnimationManager::GetInstance()->Get("ani-version")->Clone();
		animations["BottomCurtain"] = AnimationManager::GetInstance()->Get("ani-bottom-curtain-")->Clone();
	}
}
