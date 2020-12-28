#include "QuestionBlock.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"
#include "RedMushroom.h"
#include "GreenMushroom.h"
#include "RaccoonLeaf.h"
#include "PSwitch.h"
#include "Mario.h"

QuestionBlock::QuestionBlock()
{
	this->renderOrder = 1500;
	this->Gravity = 0;
	this->state = QuestionBlockStates::Available;
}

void QuestionBlock::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Avail"] = AnimationManager::GetInstance()->Get("ani-question-block")->Clone();
		this->animations["BrickAvail"] = AnimationManager::GetInstance()->Get("ani-brick")->Clone();
		this->animations["Unavail"] = AnimationManager::GetInstance()->Get("ani-empty-block")->Clone();
	}
}

void QuestionBlock::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (state != QuestionBlockStates::Available) return;

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	collisionCal->CalcPotentialCollisions(coObj, false);
}

void QuestionBlock::Hit()
{
	if (state != QuestionBlockStates::Available) return;
	Velocity.y = -2.8125f;
	Gravity = 0;
	backupPos = Position;
	this->state = QuestionBlockStates::Bouncing;

	shared_ptr<Mario> mario = SceneManager::GetInstance()->GetPlayer<Mario>();

	DebugOut(L"Reward: %s\n", ToLPCWSTR(reward.ToString()));

	if (reward == MEntityType::QuestionCoin) {
		//SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(RedMushroom::CreateRedMushroom(Position));
	}
	else if (reward == MEntityType::GreenMushroom) {
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(GreenMushroom::CreateGreenMushroom(Position));
	}
	else if (reward == MEntityType::PSwitch) {
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(PSwitch::CreatePSwitch(Position));
	}
	else if (mario->GetObjectType() != MEntityType::SmallMario) {
		if (reward == MEntityType::RedMushroom) {
			SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(RedMushroom::CreateRedMushroom(Position));
		}
		if (reward == MEntityType::RaccoonLeaf) {
			SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(RaccoonLeaf::CreateRaccoonLeaf(Position));
		}
	}
	else {
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(RedMushroom::CreateRedMushroom(Position));
	}
}

QuestionBlockStates QuestionBlock::GetState()
{
	return state;
}

void QuestionBlock::PositionUpdate()
{
	if (state != QuestionBlockStates::Bouncing) return;
	Position += Distance;

	if (Position.y < backupPos.y - MaxBounce) {
		Position.y = backupPos.y - MaxBounce;
		Velocity.y = 0;
		Gravity = 0.004f;
	}

	if (Position.y > backupPos.y) {
		Position = backupPos;
		Gravity = 0;
		Velocity = Vec2(0, 0);
		Distance = Vec2(0, 0);
		state = QuestionBlockStates::Unavailable;
	}
	UpdatedDistance = Distance;
}

void QuestionBlock::PositionLateUpdate()
{
}

bool QuestionBlock::HasCollideWith(DWORD id)
{
	return true;
}

void QuestionBlock::StatusUpdate()
{
	if (state != QuestionBlockStates::Available) return;

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

	if (coResult.size() > 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			/*if (coll->Object->GetObjectType() == MEntityType::MarioTailed 
				|| (coll->Object->GetObjectType() == MEntityType::KoopasImposter && ToVector(coll->SAABBResult.Direction).x != 0)
				|| (MEntityType::IsMario(coll->Object->GetObjectType()) && coll->SAABBResult.Direction == Direction::Top)) {*/
			if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
				Hit();
				break;
			}
		}
	}
}

void QuestionBlock::FinalUpdate()
{
	collisionCal->Clear();
}

void QuestionBlock::Update()
{
	if (state == QuestionBlockStates::Bouncing) {
		DWORD dt = CGame::Time().ElapsedGameTime;
		Velocity.y += Gravity * (float)dt;
		Distance = Velocity * (float)dt;
	}
}

void QuestionBlock::Render()
{
	InitResource();

	Animation ani = activeState == QuestionBlockActiveStates::Question ? animations["Avail"] : animations["BrickAvail"];

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

bool QuestionBlock::IsGetThrough(GameObject& object, Direction direction)
{
	return false;
}

float QuestionBlock::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<QuestionBlock> QuestionBlock::CreateQuestionBlock(Vec2 fixedPos, MapProperties& props)
{
	shared_ptr<QuestionBlock> block = make_shared<QuestionBlock>();
	block->SetCollisionCalculator(make_shared<CollisionCalculator>(block));
	block->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	block->activeState = props.GetBool("AsBrick", false) ? QuestionBlockActiveStates::GlassBrick : QuestionBlockActiveStates::Question;

	string rwType = props.GetText("HiddenItem", "QuestionCoin");
	if (rwType.compare(MEntityType::RedMushroom.ToString()) == 0) {
		block->reward = MEntityType::RedMushroom;
	}
	if (rwType.compare(MEntityType::GreenMushroom.ToString()) == 0) {
		block->reward = MEntityType::GreenMushroom;
	}
	if (rwType.compare(MEntityType::RaccoonLeaf.ToString()) == 0) {
		block->reward = MEntityType::RaccoonLeaf;
	}
	if (rwType.compare(MEntityType::PSwitch.ToString()) == 0) {
		block->reward = MEntityType::PSwitch;
	}
	if (rwType.compare(MEntityType::QuestionCoin.ToString()) == 0) {
		block->reward = MEntityType::QuestionCoin;
	}

	return block;
}
