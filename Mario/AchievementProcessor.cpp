#include "AchievementProcessor.h"
#include "GameEvent.h"
#include "MarioGame.h"
#include "PlayerData.h"
#include "EffectServer.h"
#include "CoinFX.h"

AchievementProcessor::AchievementProcessor(shared_ptr<PlayerData> data)
{
	this->data = data;

	__hook(&GameEvent::PlayerCoinChangeEvent, GameEvent::GetInstance(), &AchievementProcessor::OnPlayerCoinChange);
	__hook(&GameEvent::PlayerScoreChangeEvent, GameEvent::GetInstance(), &AchievementProcessor::OnPlayerScoreChange);
	__hook(&GameEvent::PlayerLifeChangeEvent, GameEvent::GetInstance(), &AchievementProcessor::OnPlayerLifeChange);
}

void AchievementProcessor::OnPlayerCoinChange(const char* file, int amount)
{
	data->Coins += amount;
}

void AchievementProcessor::OnPlayerScoreChange(const char* file, Score score, Vec2 Position, ScoreSource reason)
{
	data->Score += (int)score;

	if (reason == ScoreSource::QUESTION_BLOCK) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<CoinFX>(Position - Vec2(0, 48), score));
	}
	else {
		EffectServer::GetInstance()->SpawnEffect(make_shared<ScoreFX>(Position + Vec2(0, 30), score));
	}
}

void AchievementProcessor::OnPlayerLifeChange(const char* file, Vec2 Position)
{
	data->Lives += 1;
	EffectServer::GetInstance()->SpawnEffect(make_shared<ScoreFX>(Position + Vec2(0, 30), Score::S1UP));
}
