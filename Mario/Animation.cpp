#include "Animation.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Game.h"

CAnimation::CAnimation(int defaultFrameTime)
{
	this->defaultFrameTime = defaultFrameTime;
	this->playScale = 1.0f;
	this->lastFrameTime = 0;
	this->currentFrame = 0;
	this->transform = new Transform();
	this->frames.clear();
}

CAnimation::CAnimation(CAnimation* origin) : CAnimation(origin->defaultFrameTime)
{
	this->transform->Position = origin->transform->Position;
	this->transform->Scale = origin->transform->Scale;
	this->transform->Rotation = origin->transform->Rotation;
	this->frames = vector<AnimationFrame>(origin->frames);
	this->lastFrameTime = origin->lastFrameTime;
	this->currentFrame = origin->currentFrame;
	this->defaultFrameTime = origin->defaultFrameTime;
	this->playScale = origin->playScale;
}

void CAnimation::AddFrame(Sprite sprite, DWORD frameTime)
{
	frameTime = frameTime == 0 ? defaultFrameTime : frameTime;
	AnimationFrame frame = new CAnimationFrame(sprite, frameTime);
	this->frames.push_back(frame);
}

void CAnimation::Render(D3DCOLOR overlay)
{
	DWORD now = CGame::Time().TotalGameTime.Milliseconds();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if ((now - lastFrameTime)*playScale > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(transform->Position.x, transform->Position.y, *transform, overlay);
}

Animation CAnimation::Clone()
{
	return new CAnimation(this);
}

CAnimation::~CAnimation()
{
	delete this->transform;
}
