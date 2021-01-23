#include "FightProcessor.h"
#include "GameEvent.h"
#include "PlayerData.h"
#include "MarioGame.h"
#include "AchievementProcessor.h"
#include "GameObject.h"
#include "SceneManager.h"

FightProcessor::FightProcessor(shared_ptr<PlayerData> data)
{
	this->data = data;

	__hook(&GameEvent::EnemyDamagedEvent, GameEvent::GetInstance(), &FightProcessor::OnEnemyDamaged);
	__hook(&GameEvent::PlaySceneClearEvent, GameEvent::GetInstance(), &FightProcessor::OnPlaySceneClear);
}

void FightProcessor::SetEnable(bool value)
{
	this->enable = value;
}

bool FightProcessor::IsEnable()
{
	return enable;
}

void FightProcessor::OnEnemyDamaged(const char* file, DamgeSource source, Vec2 Position, ObjectType& victimType)
{
	if (!enable) return;
	switch (source)
	{
	case DamgeSource::ENEMY:
		break;

	case DamgeSource::MARIO:
		if (MarioGame::Time().TotalGameTime - last < 1000) {
			switch (lastScore)
			{
			case Score::S100:
				lastScore = Score::S200;
				break;
			case Score::S200:
				lastScore = Score::S400;
				break;
			case Score::S400:
				lastScore = Score::S800;
				break;
			case Score::S800:
				lastScore = Score::S1000;
				break;
			case Score::S1000:
				lastScore = Score::S2000;
				break;
			case Score::S2000:
				lastScore = Score::S4000;
				break;
			case Score::S4000:
				lastScore = Score::S8000;
				break;
			case Score::S8000:
				lastScore = Score::S1UP;
				break;
			case Score::S1UP:
				lastScore = Score::S1UP;
				break;
			}
		}
		else {
			lastScore = Score::S100;
		}
		last = MarioGame::Time().TotalGameTime;
		if (lastScore == Score::S1UP) {
			MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerLifeChange(file, Position);
		}
		else {
			MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerScoreChange(file, lastScore, Position, ScoreSource::DAMAGE_ENEMY);
		}
		break;

	case DamgeSource::MARIO_WEAPON:
		MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerScoreChange(file, Score::S100, Position, ScoreSource::DAMAGE_ENEMY);
		break;

	case DamgeSource::SPACE:
		if (victimType == MEntityType::KoopasImposter || victimType == MEntityType::RedKoopasImposter) {
			MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerScoreChange(file, Score::S100, Position, ScoreSource::DAMAGE_ENEMY);
		}
		break;

	case DamgeSource::SOLID_BLOCK:
		if (victimType == MEntityType::KoopasCrouch || victimType == MEntityType::RedKoopasCrouch 
			|| victimType == MEntityType::KoopasPassenger || victimType == MEntityType::RedKoopasPassenger) {
			MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerScoreChange(file, Score::S100, Position, ScoreSource::DAMAGE_ENEMY);
		}
		break;
	}
}

void FightProcessor::OnPlaySceneClear(const char* file, SceneResult result)
{
	if (!enable) return;
	if (result == SceneResult::WIN) {
		vector<shared_ptr<GameObject>> objects = SceneManager::GetInstance()->GetActiveScene()->GetUpdatingObjects();
		RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
		for each (shared_ptr<GameObject> var in objects)
		{
			if (MEntityType::IsEnemy(var->GetObjectType()) && CollisionCalculator::CommonAABB(cam, var->GetHitBox())) {				
				var->SetActive(false);
				MarioGame::GetInstance()->GetAchievementProcessor()->OnPlayerScoreChange(file, Score::S1000, var->GetPosition(), ScoreSource::SERVER_KILL);
			}
		}
	}
}
