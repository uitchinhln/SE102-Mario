#pragma once
#include "Utils.h"
#include "MapGate.h"

class Graph
{
public:
	virtual void AddNode(shared_ptr<MapGate> node);
	virtual shared_ptr<MapGate> GetNode(int id);

protected:
	unordered_map<int, shared_ptr<MapGate>> nodes;
};

