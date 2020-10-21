#pragma once
#include "Utils.h"
#include "CollisionResult.h"

class Mario;

class AbstractMarioState
{
public:
	virtual RectF GetBoundingBox();

	virtual void Update(vector<shared_ptr<CollisionResult>> collisions);

	virtual void Render();

protected:
	weak_ptr<Mario> mario;
};

