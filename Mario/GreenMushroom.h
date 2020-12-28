#pragma once
#include "RedMushroom.h"
class GreenMushroom :
	public RedMushroom
{
public:
	virtual void InitResource() override;

	virtual ObjectType GetObjectType() override;

	static shared_ptr<GreenMushroom> CreateGreenMushroom(Vec2 pos);
};

