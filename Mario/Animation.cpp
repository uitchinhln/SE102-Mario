#include "Animation.h"

CAnimation::CAnimation(int defaultFrameTime)
{
	this->defaultFrameTime = defaultFrameTime;
	this->lastFrameTime = 0;
	this->currentFrame = 0;
	this->transform = new Transform();
	this->frames.clear();
}

void CAnimation::AddFrame(Sprite sprite, DWORD frameTime)
{
	frameTime = frameTime == 0 ? defaultFrameTime : frameTime;
	AnimationFrame frame = new CAnimationFrame(sprite, frameTime);
	this->frames.push_back(frame);
}

void CAnimation::Render()
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

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}
