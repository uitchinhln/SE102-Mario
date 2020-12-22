#pragma once
#include "DefaultKoopas.h"
#include "RayCast.h"
class DefRedKoopas : public DefaultKoopas
{
public:
    DefRedKoopas(shared_ptr<Koopas> koopas);

    virtual void InitResource(bool force = false) override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual ObjectType GetObjectType() override;

	~DefRedKoopas();

protected:
	RayCast raycaster;
};

