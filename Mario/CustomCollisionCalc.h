#pragma once
#include "CollisionCalculator.h"

class CustomCollisionCalc :
    public CollisionCalculator
{
public:
    CustomCollisionCalc(weak_ptr<IColliable> object);
protected:
    virtual SweptCollisionResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false) override;
};

