#pragma once
#include "libs.h"

class Camera
{
protected:
	Transform* transform;

public:
	Camera();

	Transform* GetTransform();
};

