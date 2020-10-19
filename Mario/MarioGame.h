#pragma once
#include "Game.h";

class MarioGame : public CGame
{
public:
	MarioGame();

	virtual void LoadResources() override;
	virtual void Update() override;
	virtual void Draw() override;
};

