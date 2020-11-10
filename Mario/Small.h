#pragma once
#include "MarioPowerUp.h"
class Small :
	public MarioPowerUp
{
public:
	Small(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual void CrouchUpdate() override;

	virtual ObjectType GetMarioType() override;
protected:
	
};

