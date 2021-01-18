#include "CoinFX.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"
#include "EffectServer.h"

CoinFX::CoinFX(Vec2 pos, Score score) : IEffect(pos, 20000)
{
	Position = rootPos = pos;
	this->score = score;
}

void CoinFX::Update()
{
	Active = true;
	if (stage == 0) {
		Position.y -= flyUpSpeed * CGame::Time().ElapsedGameTime;

		if (rootPos.y - Position.y > 144) {
			stage = 1;
		}
	}
	else {
		fallDownSpeed += gravity * CGame::Time().ElapsedGameTime;
		Position.y += fallDownSpeed * CGame::Time().ElapsedGameTime;

		if (rootPos.y - Position.y < 24) {
			EffectServer::GetInstance()->SpawnEffect(make_shared<ScoreFX>(Position, score));
			Active = false;
		}
	}
}

void CoinFX::Render()
{
	if (animation == nullptr) {
		animation = AnimationManager::GetInstance()->Get("ani-coin-obtained")->Clone();
		animation->SetPlayScale(4.0f);
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	animation->GetTransform()->Position = Position - cam + size / 2;
	animation->Render();
}

