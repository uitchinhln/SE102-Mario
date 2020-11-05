#pragma once
#include "Utils.h"
#include "CollisionResult.h"
#include "Animation.h"
#include "TinyXML/tinyxml.h"
#include "MEntityType.h"


class Mario;

class MarioPowerUp
{
public:
	MarioPowerUp(shared_ptr<Mario> mario);

	virtual RectF GetHitBox();

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj);

	virtual void StatusUpdate();

	virtual void MoveUpdate();

	virtual void PowerMeterUpdate();

	virtual void MiniJumpDetect(bool forceX = false);

	virtual void JumpUpdate();

	virtual void CrouchUpdate();

	virtual void Update();

	virtual void JumpAnimation();

	virtual void MoveAnimation();

	virtual void Render();

	virtual void OnKeyDown(int key);

	virtual void OnKeyUp(int key);

	virtual void Init(TiXmlElement* data);

	virtual shared_ptr<Mario> GetMario();

	virtual ObjectType GetMarioType() = 0;

protected:
	weak_ptr<Mario> mario;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	shared_ptr<CAnimation> selectedAnimation = NULL;

	Vec2 size = Vec2(45, 81);
	Vec2 sizeFixed = Vec2(45, 81);

	int	  PMETER_MAX = 7;
	float PMETER_UP_STEP = 0.008f;
	float PMETER_DOWN_STEP = 0.0016f;
	float MARIO_GRAVITY = 0.00133f * 1.5;
	float MARIO_WALK_SPEED = 0.20f;
	float MARIO_RUN_SPEED = 0.48f;
	float MARIO_JUMP_FORCE = 0.27f * 1.2;
	float MARIO_HIGH_JUMP_FORCE = 0.40f * 1.2;
	float MARIO_SUPER_JUMP_FORCE = 0.65f * 1.3;
	float MARIO_PUSH_FORCE = 0.0019f;
	float MARIO_WALK_ACCELERATION = 0.000376f;
	float MARIO_RUN_ACCELERATION = 0.000288f;
	float MARIO_WALK_DRAG_FORCE = 0.0003006f;
	float MARIO_RUN_DRAG_FORCE = 0.0006366f;
	float MARIO_CROUCH_DRAG_FORCE = 0.0003266f;
	float MARIO_SKID_ACCELERATION = 0.001104f;
};

