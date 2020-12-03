#pragma once
#include "DefaultKoopas.h"
#include "StopWatch.h"

class CrouchKoopas :
	public DefaultKoopas
{
public:
	CrouchKoopas(shared_ptr<Koopas> koopas, bool flip = false);

	virtual void InitResource(bool force = false) override;

	virtual void FinalUpdate() override;

	virtual void HeldUpdate();

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

protected:
	bool flip = false;

	Stopwatch respawnTimer;

	long KP_RESPAWN_TIME = 8000;
};

