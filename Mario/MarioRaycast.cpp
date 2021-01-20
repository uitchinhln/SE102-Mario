#include "Mario.h"
#include "Game.h"

void Mario::OverlapUpdate()
{
	sliding = false;

	vector<shared_ptr<CollisionResult>> overlapsEvents = collisionCal->GetOverlaps();
	if (overlapsEvents.size() > 0) {
		overlapsEvents.erase(remove_if(overlapsEvents.begin(), overlapsEvents.end(), [](const shared_ptr<CollisionResult>& evt) {
			return !MEntityType::IsTile(evt->Object->GetObjectType());
			}), overlapsEvents.end());
	}

	if (overlapsEvents.size() > 0) {
		DWORD marioId = this->GetID();
		RectF hitbox = GetHitBox();
		float marioWidth = hitbox.right - hitbox.left;

		Vec2 headPoint = Vec2(hitbox.left, hitbox.top + (hitbox.bottom - hitbox.top) / 4);
		Vec2 legPoint = Vec2(hitbox.left, hitbox.bottom - (hitbox.bottom - hitbox.top) / 4);

		ObjectList headResult;
		ObjectList legResult;

		vector<shared_ptr<Vec2>> headBoxes;
		vector<shared_ptr<Vec2>> legBoxes;

		raycaster->Filter([marioId](const shared_ptr<GameObject>& obj) {
			return !MEntityType::IsTile(obj->GetObjectType()) || obj->GetID() == marioId;
			});

		raycaster->Shoot(headPoint, Direction::Right, 680, headResult);
		raycaster->Shoot(legPoint, Direction::Right, 680, legResult);

		raycaster->MergeBox(headResult, Direction::Right, marioWidth - 0.0001, headBoxes);
		raycaster->MergeBox(legResult, Direction::Right, marioWidth - 0.0001, legBoxes);

		sort(headBoxes.begin(), headBoxes.end(), [](shared_ptr<Vec2> a, shared_ptr<Vec2> b) {return a->x < b->x; });
		sort(legBoxes.begin(), legBoxes.end(), [](shared_ptr<Vec2> a, shared_ptr<Vec2> b) {return a->x < b->x; });

		Vec2 headBox = headBoxes.empty() ? Vec2(99999, -99999) : *headBoxes[0];
		Vec2 legsBox = legBoxes.empty() ? Vec2(99999, -99999) : *legBoxes[0];

		//Debug
		/*RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
		testbox.push_back(RectF(headBox.x - cam.left, headPoint.y - 10 - cam.top, abs(headBox.y - headBox.x), 20));
		testbox.push_back(RectF(legsBox.x - cam.left, legPoint.y - 10 - cam.top, abs(legsBox.y - legsBox.x), 20));*/

		Vec2 overBox = legsBox,
			barrierBox = headBox;
		if (headBox.x <= legsBox.x) {
			overBox = headBox;
			barrierBox = legsBox;
		}

		sliding = true;
		if (hitbox.left + marioWidth / 5 < overBox.x) {
			headPoint.x = min(headBox.x, hitbox.right);
			legPoint.x = min(legsBox.x, hitbox.right);

			raycaster->Shoot(headPoint, Direction::Left, 340, headResult);
			raycaster->Shoot(legPoint, Direction::Left, 340, legResult);

			raycaster->MergeBox(headResult, Direction::Left, marioWidth - 0.0001, headBoxes);
			raycaster->MergeBox(legResult, Direction::Left, marioWidth - 0.0001, legBoxes);

			sort(headBoxes.begin(), headBoxes.end(), [](shared_ptr<Vec2> a, shared_ptr<Vec2> b) {return a->y > b->y; });
			sort(legBoxes.begin(), legBoxes.end(), [](shared_ptr<Vec2> a, shared_ptr<Vec2> b) {return a->y > b->y; });

			//Debug
			/*RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
			for each (shared_ptr<Vec2> var in headBoxes)
				testbox.push_back(RectF(var->x - cam.left, headPoint.y - 10 - cam.top, abs(var->y - var->x), 20));
			for each (shared_ptr<Vec2> var in legBoxes)
				testbox.push_back(RectF(var->x - cam.left, legPoint.y - 10 - cam.top, abs(var->y - var->x), 20));*/

			headBox = headBoxes.empty() ? Vec2(99999, -99999) : *headBoxes[0];
			legsBox = legBoxes.empty() ? Vec2(99999, -99999) : *legBoxes[0];

			Vec2 overBoxL = legsBox,
				barrierBoxL = headBox;
			if (headBox.y >= legsBox.y) {
				overBoxL = headBox;
				barrierBoxL = legsBox;
			}

			if (hitbox.left < overBoxL.x) {
				Position.x -= 0.3 * CGame::Time().ElapsedGameTime;
			}
			else {
				if (barrierBox.x - overBox.y >= marioWidth || overBoxL.x - barrierBoxL.y < marioWidth) {
					Position.x += 0.3 * CGame::Time().ElapsedGameTime;
				}
				else {
					Position.x -= 0.3 * CGame::Time().ElapsedGameTime;
				}
			}
		}
		else {
			if (barrierBox.x - overBox.y >= marioWidth) {
				Position.x += 0.3 * CGame::Time().ElapsedGameTime;
			}
			else {
				Position.x -= 0.3 * CGame::Time().ElapsedGameTime;
			}
		}
	}
	raycaster->Clear();
}

void Mario::OverlapUpdateOriginal()
{
	sliding = false;

	vector<shared_ptr<CollisionResult>> overlapsEvents = collisionCal->GetOverlaps();
	if (overlapsEvents.size() > 0) {
		overlapsEvents.erase(remove_if(overlapsEvents.begin(), overlapsEvents.end(), [](const shared_ptr<CollisionResult>& evt) {
			return !MEntityType::IsTile(evt->Object->GetObjectType());
			}), overlapsEvents.end());
	}

	if (overlapsEvents.size() > 0) {
		RectF hitbox = GetHitBox();
		float marioWidth = hitbox.right - hitbox.left;

		Vec2 headPoint = Vec2(hitbox.left, hitbox.top + (hitbox.bottom - hitbox.top) / 4);

		ObjectList headResult;

		vector<shared_ptr<Vec2>> headBoxes;

		raycaster->Filter([](const shared_ptr<GameObject>& obj) {
			return !MEntityType::IsTile(obj->GetObjectType());
			});

		raycaster->Shoot(headPoint, Direction::Right, 480, headResult);

		raycaster->MergeBox(headResult, Direction::Right, marioWidth - 0.0001, headBoxes);

		if (headBoxes.size() < 1) return;

		sort(headBoxes.begin(), headBoxes.end(), [](shared_ptr<Vec2> a, shared_ptr<Vec2> b) {return a->x < b->x; });

		Vec2 headBox = *headBoxes[0];

		if (hitbox.right <= headBox.x || hitbox.left >= headBox.y) return;

		//Debug
		//RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
		//testbox.push_back(RectF(headBox.x - cam.left, headPoint.y - 10 - cam.top, abs(headBox.y - headBox.x), 20));

		if (hitbox.left + marioWidth / 5 < headBox.x) {
			Position.x -= 0.3 * CGame::Time().ElapsedGameTime;
		}
		else {
			Position.x += 0.3 * CGame::Time().ElapsedGameTime;
		}
		sliding = true;
	}
	raycaster->Clear();
}