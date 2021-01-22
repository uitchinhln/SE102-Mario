#pragma once
#include "TreeLeft.h"
class TreeRight :
    public TreeLeft
{
public:
    virtual void InitResource() override;

    static shared_ptr<TreeRight> CreateTreeRight(Vec2 fixPos = VECTOR_0);
};

