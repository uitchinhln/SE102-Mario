#include "SpriteParticle.h"
#include "SceneManager.h"

void SpriteParticle::Render()
{
	if (Sprites.size() < 1) return;

	Sprite sprite = Sprites.begin()->second;
	
	if (sprite == nullptr) return;

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
	Vec2 renderPos = Position - cam + Size / 2;

	sprite->Draw(renderPos.x, renderPos.y, this->Transform);
}

void SpriteParticle::Render(int runTime, int totalTime)
{
	this->Render();
}

bool SpriteParticle::HasResource()
{
	return Sprites.size() > 0;
}
