#pragma once
#include "Sprite.h"

class CAnimationFrame
{
	Sprite sprite;
	DWORD time;

public:
	CAnimationFrame(Sprite sprite, int time);
	virtual DWORD GetTime() { return time; }
	virtual Sprite GetSprite() { return sprite; }
};

typedef CAnimationFrame* AnimationFrame;

