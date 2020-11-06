#pragma once
#include "GameObject.h"

enum class QuestionBlockStates {
	Available,
	Bouncing,
	Unavailable
};

class QuestionBlock :
    public GameObject
{

public:
	QuestionBlock();

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

	static shared_ptr<QuestionBlock> CreateQuestionBlock(Vec2 fixedPos = Vec2(100, 100));

protected:
	Vec2 size = Vec2(48, 48);

	Vec2 backupPos = Vec2(0, 0);

	QuestionBlockStates state = QuestionBlockStates::Available;
};

