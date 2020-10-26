#pragma once
#include "MarioPowerUp.h"
class Small :
	public MarioPowerUp
{
public:
	Small(shared_ptr<Mario> mario);

	virtual RectF GetBoundingBox() override;

	virtual ObjectType GetMarioType() override;
protected:
	
};

