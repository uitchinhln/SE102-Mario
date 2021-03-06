#pragma once
#include "Utils.h"
#include "Direction.h"
#include "GameObject.h"
#include <functional>

class GameObject;

class RayCastResult;

typedef vector<shared_ptr<GameObject>> ObjectList;

class RayCast
{
public:
	void SetInput(ObjectList* objects);
	void Shoot(Vec2 startPoint, Direction direction, float distanceLimit, ObjectList& results);
	void MergeBox(ObjectList& input, Direction direction, float maxSpace, vector<shared_ptr<Vec2>>& output);
	void Clear();
	void Filter(function<bool(shared_ptr<GameObject>)> callback);

protected:
	ObjectList objects;
};

