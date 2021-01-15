#include "Grid.h"
#include "GameObject.h"
#include "MEntityType.h"

Grid::Grid(int col, int row, float cellWidth, float cellHeight)
{
	this->col = col;
	this->row = row;
	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;

	grid = new vector<DWORD> * [col];
	for (int i = 0; i < col; i++) {
		grid[i] = new vector<DWORD>[row];
	}
}

void Grid::Add(DWORD objectID, int x, int y)
{
	grid[x][y].push_back(objectID);
}

void Grid::Add(shared_ptr<GameObject> object, int excludeX, int excludeY)
{
	RectF objBox = object->GetHitBox();
	RectF byCell(objBox.left / cellWidth, objBox.top / cellHeight, objBox.right / cellWidth, objBox.bottom / cellHeight);

	int left = (int)floor(byCell.left);
	left = max(0, left - (floor(byCell.left) == left));
	int top = (int)floor(byCell.top);
	top = max(0, top - (floor(byCell.top) == top));

	int right = (int)ceil(byCell.right);
	right = min(col, right + (ceil(byCell.right) == right));
	int bottom = (int)ceil(byCell.bottom);
	bottom = min(row, bottom + (ceil(byCell.bottom) == bottom));

	for (int i = left; i < right; i++) {
		for (int j = top; j < bottom; j++) {
			if (i == excludeX && j == excludeY) continue;
			grid[i][j].push_back(object->GetID());
		}
	}
}

void Grid::GetByCamera(shared_ptr<Camera> cam, unordered_map<DWORD, shared_ptr<GameObject>>& objects, vector<shared_ptr<GameObject>>& result)
{
	RectF camBox = cam->GetBoundingBox();
	unordered_map<DWORD, DWORD> checked;

	int beginX = (int)floor(camBox.left / cellWidth);
	int beginY = (int)floor(camBox.top / cellHeight);
	int endX = (int)ceil(camBox.right / cellWidth);
	int endY = (int)ceil(camBox.bottom / cellHeight);

	beginX = max(0, beginX);
	beginY = max(0, beginY);
	endX = min(col, endX);
	endY = min(row, endY);

	unordered_map<DWORD, bool> available;

	for (int i = beginX; i < endX; i++) {
		for (int j = beginY; j < endY; j++) {
			if (grid[i][j].size() < 1) continue;

			RectF cellBox = RectF(i * cellWidth, j * cellHeight, i * cellWidth + cellWidth, j * cellHeight + cellHeight);

			available.clear();
			for each (DWORD objectID in grid[i][j])
			{
				if (objects.find(objectID) == objects.end()) continue;
				if (available.find(objectID) != available.end() && available.at(objectID) == false) continue;

				shared_ptr<GameObject> object = objects.at(objectID);
				
				if (object == nullptr) continue;

				RectF objBox = object->GetHitBox();
				if (!CollisionCalculator::CommonAABB(cellBox, objBox)) {
					DebugOut(L"Refactor object[%d]: %s\n", object->GetID(), ToLPCWSTR(object->GetObjectType().ToString())); //BUG
					Add(object, i , j);
					available[objectID] = false;
					continue;
				}
				available[objectID] = true;

				if (checked.find(objectID) == checked.end()) {
					checked[objectID] = objectID;
					if (object->IsActive()) {
						result.push_back(object);
					}
				}
			}
			grid[i][j].clear();
			for each (auto & iter in available)
			{
				if (iter.second == true) {
					grid[i][j].push_back(iter.first);
				}
			}
		}
	}
}

void Grid::Clear(int col, int row)
{
	grid[col][row].clear();
}

void Grid::ClearAll()
{
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			grid[i][j].clear();
		}
	}
}
