#pragma once
#include "Utils.h"
#include "Transform.h"

[event_receiver(native)]
class Camera
{
protected:
	Vec2 size;


public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual void Update();

	virtual void OnKeyUp(int key);

	virtual void OnKeyDown(int key);

	virtual void HookEvent();

	virtual void UnHookEvent();

	~Camera();
};

