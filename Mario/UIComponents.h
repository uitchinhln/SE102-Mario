#pragma once
#include "Utils.h"
#include "Animation.h"

class UIComponents
{
protected:
	unordered_map<string, Animation> animations;
	unordered_map<string, Sprite> sprites;

public:
	Vec2 Position;
	Vec2 Size;

	virtual void InitResource();

	virtual void Update() = 0;

	virtual void Render() = 0;
};

