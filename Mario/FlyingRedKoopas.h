#pragma once
#include "DefRedKoopas.h"
class FlyingRedKoopas :
    public DefaultKoopas
{
protected:
	float range = 144;

	Vec2 rootPos;

public:

	FlyingRedKoopas(shared_ptr<Koopas> koopas, Vec2 position, float range);

	virtual void InitResource(bool force = false) override;

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void OnDeath(Vec2 veloc) override;

	virtual ObjectType GetObjectType() override;

};

