#include "ObjectType.h"

ObjectType ObjectType::CreateObjectType(string name)
{
	return ObjectType(name);
}

bool ObjectType::operator==(const ObjectType& target)
{
	return this->name == target.name;
}

bool ObjectType::operator!=(const ObjectType& target)
{
	return !(this->name == target.name);
}

bool ObjectType::operator==(const string& name)
{
	return this->name == name;
}

bool ObjectType::operator!=(const string& name)
{
	return !(this->name == name);
}

string ObjectType::ToString()
{
	return name;
}

ObjectType::ObjectType(string name) {
	this->name = name;
}