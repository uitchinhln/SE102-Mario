#pragma once
#include "Utils.h"
#include "Transform.h"
#include "AnimationFrame.h"

class CAnimation :
	public enable_shared_from_this<CAnimation>
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultFrameTime;

	float playScale;

	Transform* transform;

	vector<AnimationFrame> frames;

public:
	CAnimation(int defaultFrameTime = 100);
	CAnimation(shared_ptr<CAnimation> origin);

	virtual Transform* GetTransform() { return this->transform; }

	virtual void AddFrame(Sprite sprite, DWORD frameTime = 0);

	virtual void Render(D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	virtual void Render(int runTime, int totalTime, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	virtual float GetPlayScale() { return this->playScale; }

	virtual void SetPlayScale(float scale) { this->playScale = scale; }

	virtual shared_ptr<CAnimation> Clone();

	~CAnimation();
};

typedef shared_ptr<CAnimation> Animation;
