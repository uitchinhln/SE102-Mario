#include "IEffect.h"
#include "Game.h"
#include "SceneManager.h"
#include "CollisionCalculator.h"

IEffect::IEffect(Vec2 Position)
{
    this->Position = Position;
}

IEffect::IEffect(Vec2 Position, long playtime) : IEffect(Position)
{
    this->playtimeLeft = playtime;
}

void IEffect::InitResource(Particle* particle)
{
}

Vec2 IEffect::GetPosition()
{
    return this->Position;
}

long IEffect::GetPlayTimeLeft()
{
    return playtimeLeft;
}

void IEffect::SetPlayTimeLeft(long value)
{
    this->playtimeLeft = value;
}

void IEffect::Update()
{
    Active = false;

    playtimeLeft -= CGame::Time().ElapsedGameTime;
    if (playtimeLeft <= 0) return;

    RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;

        particle->Update();
        if (CollisionCalculator::CommonAABB(cam, particle->GetBoundingBox())) {
            Active = true;
        }
    }
}

void IEffect::Render(D3DXCOLOR overlay)
{
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        if (!particle->HasResource()) {
            InitResource(particle);
        }
        particle->Render(overlay);
    }
}

IEffect::~IEffect()
{
    for each (Particle* particle in particles)
    {
        delete particle;
    }
}
