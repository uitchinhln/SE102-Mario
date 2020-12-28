#pragma once
#include "UIComponents.h"
#include "Sprite.h"

class PowerMeter : 
	public UIComponents
{
public:
	PowerMeter();

	void Update();

	void Render() override;

	void SetLevel(int level);

protected:
	Sprite arrowOn, powerOn;
	Sprite arrowOff, powerOff;
	int spacing;
	int blink;
	int level;

	float BLINK_TIME = 75;
};

