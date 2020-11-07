#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "MarioPowerUp.h"

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

[event_receiver(native)]
class Mario : 
	public GameObject,
	public enable_shared_from_this<Mario>
{
private:

protected:
	bool controllable = true;

	bool onGround = true;

	float drag = 0;

	int skid = 0;

	int canHighJump = true;

	float powerMeter = 0;

	Vec2 accelerate;

	MovingStates movingState = MovingStates::IDLE;

	JumpingStates jumpingState = JumpingStates::IDLE;

	shared_ptr<MarioPowerUp> powerUp;

	weak_ptr<GameObject> inhand;

	virtual void HookEvent();

	virtual void UnHookEvent();
public:
	Mario();

	virtual void SetPowerUp(shared_ptr<MarioPowerUp> powerUp);

	virtual void InitResource() override;

	virtual void StatusUpdate() override;

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual bool IsLockController();

	virtual void SetLockController(bool value = true);

	virtual void ClearInhand();

	virtual void SetInhand(shared_ptr<GameObject> object);

	virtual shared_ptr<GameObject> GetInhand();

	virtual float& GetDrag();

	virtual void SetDrag(float drag);

	virtual int& GetSkid();

	virtual void SetSkid(int skid);

	virtual int& CanHighJump();

	virtual void SetCanHighJump(bool value);

	virtual float& GetPowerMeter();

	virtual void SetPowerMeter(float value);
	
	virtual bool IsOnGround();

	virtual void SetOnGround(bool value);

	virtual Vec2& GetDistance() override;

	virtual Vec2& GetAccelerate();

	virtual RectF GetHitBox() override;

	virtual MovingStates& GetMovingState();

	virtual void SetMovingState(MovingStates state);

	virtual JumpingStates& GetJumpingState();

	virtual void SetJumpingState(JumpingStates state);

	virtual ObjectType GetObjectType();

	virtual void OnKeyUp(int key);

	virtual void OnKeyDown(int key);

	bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;
};

