#pragma once
#include "Utils.h"
#include "Transform.h"

class Camera
{
protected:
	Vec2 position;
	Vec2 size;

public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 GetPosition();

	Vec2 GetCamSize();

	void Update(int dt);
};

