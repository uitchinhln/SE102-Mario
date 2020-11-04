#pragma once
#include "DefaultKoopas.h"

class MovingShell : 
	public DefaultKoopas
{
public:
	MovingShell(shared_ptr<Koopas> koopas, bool flip = false);

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

protected:
	bool flip = false;

	Stopwatch respawnTimer;

	long KP_RESPAWN_TIME = 8000;
};

