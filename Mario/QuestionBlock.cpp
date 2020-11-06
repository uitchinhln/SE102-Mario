#include "QuestionBlock.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "CustomCollisionCalc.h"

QuestionBlock::QuestionBlock()
{
	this->animations["Avail"] = AnimationManager::GetInstance()->Get("ani-question-block")->Clone();
	this->animations["Unavail"] = AnimationManager::GetInstance()->Get("ani-empty-block")->Clone();

	this->Gravity = 0;
	this->state = QuestionBlockStates::Available;
}

void QuestionBlock::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	if (state != QuestionBlockStates::Available) return;

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	collisionCal->CalcPotentialCollisions(coObj, false);
}

void QuestionBlock::StatusUpdate()
{
	if (state != QuestionBlockStates::Available) return;

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

	if (coResult.size() > 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (coll->GameColliableObject->GetObjectType() == MEntityType::MarioTailed 
				|| (coll->GameColliableObject->GetObjectType() == MEntityType::KoopasImposter && ToVector(coll->SAABBResult.Direction).x != 0)
				|| (MEntityType::IsMario(coll->GameColliableObject->GetObjectType()) && coll->SAABBResult.Direction == Direction::Top)) {
				Velocity.y = -0.8f;
				Gravity = 0.006f;
				backupPos = Position;
				this->state = QuestionBlockStates::Bouncing;
				break;
			}
		}
	}
}

void QuestionBlock::FinalUpdate()
{
	if (state != QuestionBlockStates::Bouncing) return;
	GameObject::FinalUpdate();
}

void QuestionBlock::Update()
{
	if (state == QuestionBlockStates::Bouncing) {
		DWORD dt = CGame::Time().ElapsedGameTime;
		Velocity.y += Gravity * dt;
		Distance = Velocity * dt;

		if (Position.y > backupPos.y) {
			Position = backupPos;
			Gravity = 0;
			Velocity = Vec2(0, 0);
			Distance = Vec2(0, 0);
			state = QuestionBlockStates::Unavailable;
		}
	}
}

void QuestionBlock::Render()
{
	Animation ani = animations["Avail"];

	if (state != QuestionBlockStates::Available) {
		ani = animations["Unavail"];
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = GetPosition() - cam;
	ani->Render();
}

ObjectType QuestionBlock::GetObjectType()
{
	return MEntityType::QuestionBlock;
}

RectF QuestionBlock::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool QuestionBlock::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float QuestionBlock::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<QuestionBlock> QuestionBlock::CreateQuestionBlock(Vec2 fixedPos)
{
	shared_ptr<QuestionBlock> block = make_shared<QuestionBlock>();
	block->SetCollisionCalculator(make_shared<CustomCollisionCalc>(block));
	block->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	return block;
}
