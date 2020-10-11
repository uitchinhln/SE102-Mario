#pragma once
#include "libs.h"
#include "Game.h"
#include "AnimationFrame.h"
#include "Sprite.h"
#include "SpriteManager.h"

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultFrameTime;

	Transform* transform;

	vector<AnimationFrame> frames;

public:
	CAnimation(int defaultFrameTime = 100);
	CAnimation(CAnimation* origin);

	virtual Transform* GetTransform() { return this->transform; }

	virtual void AddFrame(Sprite sprite, DWORD frameTime = 0);

	virtual void Render(D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	virtual CAnimation* Clone();
};

typedef CAnimation* Animation;
