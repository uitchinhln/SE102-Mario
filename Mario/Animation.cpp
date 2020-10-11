#include "Animation.h"

CAnimation::CAnimation(int defaultFrameTime)
{
	this->defaultFrameTime = defaultFrameTime;
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
}

void CAnimation::AddFrame(Sprite sprite, DWORD frameTime)
{
	frameTime = frameTime == 0 ? defaultFrameTime : frameTime;
	AnimationFrame frame = new CAnimationFrame(sprite, frameTime);
	this->frames.push_back(frame);
}

void CAnimation::Render(D3DCOLOR overlay)
{
	DWORD now = CGame::GetInstance()->CurrentGameTime();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(
		this->transform->Position.x, this->transform->Position.y, 
		this->transform->Scale, this->transform->Rotation,
		overlay);
}

Animation CAnimation::Clone()
{
	return new CAnimation(this);
}
