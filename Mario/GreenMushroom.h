#pragma once
#include "RedMushroom.h"
class GreenMushroom :
	public RedMushroom
{
public:
	virtual void InitResource() override;

	virtual ObjectType GetObjectType() override;
};

