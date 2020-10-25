#include "EntityType.h"

EntityType EntityType::CreateEntityType(string name)
{
	return EntityType(name);
}

bool EntityType::operator==(const EntityType& target)
{
	return this->name == target.name;
}

bool EntityType::operator!=(const EntityType& target)
{
	return !(this->name == target.name);
}

bool EntityType::operator==(const string& name)
{
	return this->name == name;
}

bool EntityType::operator!=(const string& name)
{
	return !(this->name == name);
}

string EntityType::ToString()
{
	return name;
}

EntityType::EntityType(string name) {
	this->name = name;
}