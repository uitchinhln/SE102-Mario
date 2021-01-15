#pragma once
#include "Utils.h"
#include "Camera.h"

class Grid
{
public:
	Grid(int col, int row, float cellWidth, float cellHeight);

	void Add(DWORD objectID, int x, int y);

	void Add(shared_ptr<GameObject> object, int excludeX = -1, int excludeY = -1);

	void GetByCamera(shared_ptr<Camera> cam, unordered_map<DWORD, shared_ptr<GameObject>>& objects, vector<shared_ptr<GameObject>>& result);

	void Clear(int col, int row);

	void ClearAll();
protected:
	vector<DWORD>** grid;
	int col; 
	int row;
	float cellWidth;
	float cellHeight;
};

