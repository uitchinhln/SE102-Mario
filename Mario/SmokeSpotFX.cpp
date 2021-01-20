#include "SmokeSpotFX.h"
#include "AnimationManager.h"
#include "CollisionCalculator.h"
#include "SceneManager.h"
#include "Game.h"

SmokeSpotFX::SmokeSpotFX(Vec2 position) : IEffect(position, 300)
{
	Particle* particle = new Particle();
	particle->Position = position;
	particle->Size = Vec2(48, 48);
	particle->Gravity = 0;
    particle->Velocity = VECTOR_0;

	this->particles.push_back(particle);
}

void SmokeSpotFX::Update()
{
    Active = false;

    runTime += CGame::Time().ElapsedGameTime;
    if (runTime > playtimeLeft) return;

    RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        if (CollisionCalculator::CommonAABB(cam, particle->GetBoundingBox())) {
            particle->Update();
            Active = true;
        }
    }
}

void SmokeSpotFX::Render(D3DCOLOR overlay)
{
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        if (particle->Animations.size() < 1) {
            InitResource(particle);
        }
        particle->Render(runTime, playtimeLeft, overlay);
    }
}

void SmokeSpotFX::InitResource(Particle* particle)
{
	particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-fireball-damaged")->Clone();
}

SmokeSpotFX::~SmokeSpotFX()
{
    DebugOut(L"Huy SmokeSpotFX\n");
}
