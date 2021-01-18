#include "MarioFX.h"
#include "Game.h"
#include "SceneManager.h"
#include "Mario.h"

MarioFX::MarioFX(Vec2 pos, shared_ptr<Mario> mario) : IEffect(pos)
{
	this->mario = mario;
    timer.Start();
}

void MarioFX::Update()
{
    Active = false;

    if (timer.Elapsed() > playtimeLeft) return;

    RectF hitbox = mario->GetHitBox();
    Vec2 fixedPos = Vec2(hitbox.left, hitbox.bottom);

    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        
        float mid = particle->Size.x / 2 - (hitbox.right - hitbox.left) / 2;

        particle->Position = fixedPos - Vec2(mid, particle->Size.y);
    }

    Active = true;
}

void MarioFX::Render()
{
    for each (Particle * particle in particles)
    {
        if (particle == NULL) continue;
        if (!particle->HasResource()) {
            InitResource(particle);
        }
        particle->Animations.begin()->second->GetTransform()->Scale = Vec2((float)mario->GetFacing(), 1);
        particle->Render(timer.Elapsed(), playtimeLeft);
    }
}
