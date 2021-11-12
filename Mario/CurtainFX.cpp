#include "CurtainFX.h"
#include "MarioGame.h"

CurtainFX::CurtainFX(long playTime, function<void(long)> callback) : ICallbackEffect(VECTOR_0, playTime, callback)
{
	playtimeLeft = playTime;
}

void CurtainFX::Update()
{
	playtimeLeft -= MarioGame::Time().ElapsedGameTime;
	Position.y -= 0.3 * MarioGame::Time().ElapsedGameTime;

	if (!called && Position.y < -size.y / 2) {
		callback(0);
		called = true;
	}

	if (Position.y + size.y < 0) {
		callback(1);
		Active = false;
	}
}

void CurtainFX::Render(D3DXCOLOR overlay)
{
	InitResource();

	animations["Default"]->GetTransform()->Position = Position + size / 2;
	animations["Default"]->Render(overlay);
}

void CurtainFX::InitResource()
{
	if (animations.size() < 1) {
		animations["Default"] = AnimationManager::GetInstance()->Get("ani-full-curtain-")->Clone();
	}
}
