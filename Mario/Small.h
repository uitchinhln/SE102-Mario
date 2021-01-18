#pragma once
#include "MarioPower.h"
class Small :
	public MarioPower
{
public:
	Small(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual void CrouchUpdate() override;

	virtual ObjectType GetMarioType() override;


	virtual void OnDamaged(float damage) override;

	virtual void OnPowerUp(ObjectType powerType) override;
protected:
	
};

