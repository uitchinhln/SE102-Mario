#include "TreeFX.h"
#include "AnimationManager.h"

TreeFX::TreeFX(Vec2 position) : IEffect(position, 3000)
{
    Particle* particle = new Particle();
    particle->Position = position;
    particle->Size = Vec2(48, 48);
    particle->Gravity = 0;

    this->particles.push_back(particle);
}

void TreeFX::Update()
{
}

void TreeFX::Render()
{
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        if (!particle->HasResource()) {
            InitResource(particle);
        }
        particle->Render();
    }
}

void TreeFX::InitResource(Particle* particle)
{
    particle->Animations["Default"] = AnimationManager::GetInstance()->Get("ani-tree")->Clone();
}
