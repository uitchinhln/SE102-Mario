#include "Particle.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Game.h"

void Particle::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	Velocity.y += Gravity * dt;
	Position += Velocity * dt;
}

void Particle::Render(D3DXCOLOR overlay)
{
	if (Animations.size() < 1) return;
	
	Animation ani = Animations.begin()->second;

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = Position - cam + Size / 2;
	ani->Render(overlay);
}

void Particle::Render(int runTime, int totalTime, D3DXCOLOR overlay)
{
	if (Animations.size() < 1) return;

	Animation ani = Animations.begin()->second;

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = Position - cam + Size / 2;
	ani->Render(runTime, totalTime, overlay);
}

RectF Particle::GetBoundingBox()
{
	return RectF(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y);
}

bool Particle::HasResource()
{
	return Animations.size() > 0;
}
