#pragma once
#include "Position.h"

class Camera
{
protected:
	LPPosition position;

public:
	Camera();

	LPPosition GetPosition();
};

