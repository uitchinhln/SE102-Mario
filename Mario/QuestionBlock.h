#pragma once
#include "GameObject.h"
#include "MapProperties.h"

enum class QuestionBlockStates {
	Available,
	Bouncing,
	Unavailable
};

enum class QuestionBlockActiveStates {
	Question,
	GlassBrick
};

class QuestionBlock :
    public GameObject
{

public:
	QuestionBlock();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void Hit();

	virtual QuestionBlockStates GetState();

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<QuestionBlock> CreateQuestionBlock(Vec2 fixedPos, MapProperties& props);

protected:
	Vec2 size = Vec2(48, 48);

	Vec2 backupPos = Vec2(0, 0);

	float MaxBounce = 37;

	QuestionBlockStates state = QuestionBlockStates::Available;
	QuestionBlockActiveStates activeState = QuestionBlockActiveStates::Question;
};

