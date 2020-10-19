#include "GameObject.h"


void CGameObject::CalcPotentialCollisions(vector<shared_ptr<IColliable>>* objects, vector<CollisionResult>& results)
{
	for (int i = 0; i < objects->size(); i++)
	{
		CollisionResult result = SweptAABBEx(objects->at(i));

		DebugOut(L"t=%f\n", result.SAABBResult.TimeToCollide);

		if (result.SAABBResult.TimeToCollide > 0 && result.SAABBResult.TimeToCollide <= 1.0f)
			results.push_back(result);
	}
	sort(results.begin(), results.end(), CollisionResult::Compare);

}

CollisionResult CGameObject::SweptAABBEx(shared_ptr<IColliable> coO)
{
	SweptAABBResult aabbResult = IColliable::SweptAABB(GetBoundingBox(), GetDistance() - coO->GetDistance(), coO->GetBoundingBox());
	CollisionResult result = CollisionResult(aabbResult, coO);
	return result;
}

CGameObject::CGameObject()
{
	InitResource();
}

Vec2 CGameObject::GetPosition()
{
	return Position;
}

Direction CGameObject::GetDirection()
{
	return direction;
}

void CGameObject::SetDirection(Direction direction)
{
	this->direction = direction;
}

void CGameObject::InitResource()
{
}

void CGameObject::Update(vector<shared_ptr<IColliable>>* coObj)
{

}

Vec2 CGameObject::GetDistance()
{
	return Velocity;
}

CGameObject::~CGameObject()
{
}
