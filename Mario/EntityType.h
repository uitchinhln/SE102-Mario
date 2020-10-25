#pragma once
#include "Utils.h"

class EntityType
{
public:
	static EntityType CreateEntityType(string name);

	bool operator==(const EntityType& target);

	bool operator!=(const EntityType& target);

	bool operator==(const string& name);

	bool operator!=(const string& name);

	string ToString();
private:
	EntityType(string name);

	string name;
};

