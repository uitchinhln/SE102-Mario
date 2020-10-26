#pragma once
#include "MarioPowerUp.h"

class BigMario :
    public MarioPowerUp
{
public:
	BigMario(shared_ptr<Mario> mario);

	virtual ObjectType GetMarioType() override;
};

