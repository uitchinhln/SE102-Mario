#pragma once
#include "MarioPowerUp.h"

class BigMario :
    public MarioPowerUp
{
public:
	BigMario(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual ObjectType GetMarioType() override;
};

