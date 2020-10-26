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

	virtual RectF GetBoundingBox();

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj);

	virtual void MoveUpdate();

	virtual void JumpUpdate();

	virtual void Update(vector<shared_ptr<IColliable>>* coObj);

	virtual void Render();

	virtual void OnKeyDown(int key);

	virtual void OnKeyUp(int key);

	virtual void Init(TiXmlElement* data);

	virtual shared_ptr<Mario> GetMario();

	virtual ObjectType GetMarioType() = 0;

protected:
	weak_ptr<Mario> mario;

	unordered_map<string, CAnimation*> animations;

	int	  PMETER_MAX = 7;
	float PMETER_STEP = 0.1f;
	float MARIO_GRAVITY = 0.00193f;
	float MARIO_WALK_SPEED = 0.12f;
	float MARIO_RUN_SPEED = 0.40f;
	float MARIO_JUMP_FORCE = 0.25f;
	float MARIO_HIGH_JUMP_FORCE = 0.56f;
	float MARIO_SUPER_JUMP_FORCE = 0.63f;
	float MARIO_PUSH_FORCE = 0.00093f;
	float MARIO_WALK_ACCELERATION = 0.000376f;
	float MARIO_RUN_ACCELERATION = 0.000288f;
	float MARIO_WALK_DRAG_FORCE = 0.0002906f;
	float MARIO_RUN_DRAG_FORCE = 0.0006266f;
	float MARIO_SKID_ACCELERATION = 0.001104f;
};

