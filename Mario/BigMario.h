#pragma once
#include "MarioPower.h"

class BigMario :
    public MarioPower
{
public:
	BigMario(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual ObjectType GetMarioType() override;
};

