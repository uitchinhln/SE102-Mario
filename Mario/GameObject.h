#pragma once
class CGameObject
{
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};

typedef CGameObject* GameObject;

