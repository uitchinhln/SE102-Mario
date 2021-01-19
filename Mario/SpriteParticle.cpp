#include "SpriteParticle.h"
#include "SceneManager.h"

void SpriteParticle::Render(D3DCOLOR overlay)
{
	if (Sprites.size() < 1) return;

	Sprite sprite = Sprites.begin()->second;
	
	if (sprite == nullptr) return;

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
	Vec2 renderPos = Position - cam + Size / 2;

	sprite->Draw(renderPos.x, renderPos.y, this->Transform, overlay);
}

void SpriteParticle::Render(int runTime, int totalTime, D3DCOLOR overlay)
{
	this->Render(overlay);
}

bool SpriteParticle::HasResource()
{
	return Sprites.size() > 0;
}
