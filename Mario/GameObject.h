#pragma once
class CGameObject
{
	int id;

public:
	virtual void Update(int dt) = 0;
	virtual void Render() = 0;
};

typedef CGameObject* GameObject;

