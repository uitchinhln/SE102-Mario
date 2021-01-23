#pragma once
#include "MarioPower.h"
class BigLuigi :
    public MarioPower
{
public:
	BigLuigi(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual ObjectType GetMarioType() override;
};

