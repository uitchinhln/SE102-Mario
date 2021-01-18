#include "ScoreFX.h"
#include "SpriteParticle.h"
#include "SpriteManager.h"

ScoreFX::ScoreFX(Vec2 pos, Score score) : IEffect(pos, 500)
{
	SpriteParticle* particle = new SpriteParticle();
	particle->Position = pos - Vec2(0, 48);
	particle->Size = Vec2(48, 48);
	particle->Velocity = Vec2(0, -0.2f);
	particle->Sprites["Default"] = GetSprite(score);

	this->particles.push_back(particle);
}

CSprite* ScoreFX::GetSprite(Score score)
{
	switch (score)
	{
	case Score::S100:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-100");
	case Score::S200:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-200");
	case Score::S400:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-400");
	case Score::S800:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-800");
	case Score::S1000:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-1000");
	case Score::S2000:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-2000");
	case Score::S4000:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-4000");
	case Score::S8000:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-8000");
	case Score::S1UP:
		return SpriteManager::GetInstance()->Get("spr-points-in-level-1UP");
	}
	return nullptr;
}

void ScoreFX::InitResource(Particle* particle)
{
}

ScoreFX::~ScoreFX()
{
	DebugOut(L"Huy ScoreFX\n");
}
