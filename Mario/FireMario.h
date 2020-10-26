#pragma once
#include "MarioPowerUp.h"
class FireMario :
    public MarioPowerUp
{
public:
	FireMario(shared_ptr<Mario> mario);

	virtual ObjectType GetMarioType() override;
};

