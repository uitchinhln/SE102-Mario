#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "MarioPower.h"
#include "RayCast.h"
#include "MarioUtils.h"
#include "IEffect.h"
#include "StopWatch.h"
#include "PlayerData.h"

#define INVULNERABLE_TIME 2000

enum class MovingStates {
	IDLE,
	WALK,
	RUN,
	CROUCH
};

enum class JumpingStates {
	IDLE,
	JUMP,
	HIGH_JUMP,
	SUPER_JUMP,
	FALLING,
	FLOATING
};

enum class WarpStates {
	NONE,
	VERTICAL,
	HORIZONTAL
};

[event_receiver(native)]
class Mario : 
	public GameObject,
	public enable_shared_from_this<Mario>
{
private:

protected:
	bool controllable = true;

	bool sliding = false;

	bool onGround = true;

	long invulnerable = 0;

	long freezeTime = 0;

	Stopwatch freezeTimer = Stopwatch(TimeMode::SYSTEM_TIME);

	float drag = 0;

	int skid = 0;

	int kickCountDown = 0;

	float jumpBeginPos = 0;

	float powerMeter = 0;

	Vec2 accelerate;

	MovingStates movingState = MovingStates::IDLE;

	JumpingStates jumpingState = JumpingStates::IDLE;

	WarpStates warpState = WarpStates::NONE;

	shared_ptr<MarioPower> power;

	weak_ptr<GameObject> inhand;

	shared_ptr<RayCast> raycaster;

	shared_ptr<PlayerData> data;

public:
	RectF MovingBound;

public:
	Mario(shared_ptr<PlayerData> data);

	virtual shared_ptr<PlayerData> GetPlayerData();

	virtual void SetPowerUp(shared_ptr<MarioPower> power);

	virtual void InitResource() override;

	virtual void OverlapUpdate();

	virtual void OverlapUpdateOriginal();

	virtual void StatusUpdate() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual shared_ptr<RayCast> GetRayCaster();

	virtual bool IsControllerLocked();

	virtual void SetLockController(bool value = true);

	virtual void DropShell();

	virtual void ClearInhand();

	virtual void SetInhand(shared_ptr<GameObject> object);

	virtual shared_ptr<GameObject> GetInhand();

	virtual float& GetDrag();

	virtual void SetDrag(float drag);

	virtual int& GetSkid();

	virtual void SetSkid(int skid);

	virtual int& GetKickCountDown();

	virtual void SetKickCountDown(int duration);

	virtual float& GetJumpBeginPosition();

	virtual void SetJumpBeginPosition(float value);

	virtual float& GetPowerMeter();

	virtual void SetPowerMeter(float value);
	
	virtual bool IsOnGround();

	virtual void SetOnGround(bool value);

	virtual long Invulnerable();

	virtual void SetInvulnerable(long value);

	virtual Vec2& GetDistance() override;

	virtual Vec2& GetAccelerate();

	virtual RectF GetHitBox() override;

	virtual MovingStates& GetMovingState();

	virtual void SetMovingState(MovingStates state);

	virtual JumpingStates& GetJumpingState();

	virtual void SetJumpingState(JumpingStates state);

	virtual WarpStates& GetWarpState();

	virtual void SetWarpState(WarpStates state);

	virtual ObjectType GetObjectType();

	bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;



	virtual void HookEvent();

	virtual void UnHookEvent();



	virtual void OnKeyUp(int key);

	virtual void OnKeyDown(int key);

	virtual void OnGetScore(const char* source, shared_ptr<IEffect>& effect, Score score);

	virtual void OnGetCoin(const char* source, int value);

	virtual void OnDeath();

	virtual void OnDamaged(float damage);

	virtual void OnPowerUp(ObjectType powerType);
};

