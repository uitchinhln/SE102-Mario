#pragma once
#include "Utils.h"
#include "Transform.h"

[event_receiver(native)]
class Camera
{
protected:
	Vec2 size;

	virtual void HookEvent();
	virtual void UnHookEvent();

public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual void Update(int dt);

	virtual void OnKeyUp(int key);

	virtual void OnKeyDown(int key);

	~Camera();
};

