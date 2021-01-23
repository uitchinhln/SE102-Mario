#pragma once
#include "MarioPower.h"
class BigLuigi :
    public MarioPower
{
public:
	BigLuigi(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual ObjectType GetMarioType() override;



	virtual void OnDamaged(float damage) override;

	virtual void OnPowerUp(ObjectType powerType) override;
};

