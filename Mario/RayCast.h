#pragma once
#include "Utils.h"
#include "Direction.h"
#include "GameObject.h"

class GameObject;

class RayCastResult;

typedef vector<shared_ptr<GameObject>> ObjectList;

class RayCast
{
public:
	void SetInput(ObjectList* objects);
	void Shoot(Vec2 startPoint, Direction direction, float distanceLimit, ObjectList& results);
	void MergeBox(ObjectList& input, Direction direction, vector<shared_ptr<Vec2>>& output);

protected:
	ObjectList* objects;
};

