#pragma once
#include "Game.h";

class MarioGame : public CGame
{
protected:
	virtual void LoadDefaultFont();

public:
	MarioGame();

	virtual void LoadResources() override;
	virtual void Update() override;
	virtual void Draw() override;
};

