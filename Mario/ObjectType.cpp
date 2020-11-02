#include "ObjectType.h"

ObjectType ObjectType::CreateObjectType(string name)
{
	return ObjectType(name);
}

bool ObjectType::operator==(const ObjectType& target)
{
	return this->name.compare(target.name) == 0;
}

bool ObjectType::operator!=(const ObjectType& target)
{
	return this->name.compare(target.name) != 0;
}

bool ObjectType::operator==(const string& name)
{
	return this->name.compare(name) == 0;
}

bool ObjectType::operator!=(const string& name)
{
	return this->name.compare(name) != 0;
}

string ObjectType::ToString()
{
	return name;
}

string ObjectType::ToString() const
{
	return name;
}

ObjectType::ObjectType(string name) {
	this->name = name;
}