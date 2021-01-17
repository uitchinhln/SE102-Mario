#pragma once
#include "DefaultKoopas.h"
class JumpingKoopas :
    public DefaultKoopas
{
public:
    JumpingKoopas(shared_ptr<Koopas> koopas);

	virtual void InitResource(bool force = false) override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

protected:
	float KOOPAS_JUMP_FORCE = 0.475f;
};

