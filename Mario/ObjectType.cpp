#include "ObjectType.h"

ObjectType ObjectType::CreateObjectType(int id, string name)
{
	return ObjectType(id, name);
}

bool ObjectType::operator==(const ObjectType& target)
{
	return this->id == target.id;
}

bool ObjectType::operator!=(const ObjectType& target)
{
	return this->id != target.id;
}

bool ObjectType::operator==(const int& id)
{
	return this->id == id;
}

bool ObjectType::operator!=(const int& id)
{
	return this->id != id;
}

bool ObjectType::operator==(const string& name)
{
	return this->name.compare(name) == 0;
}

bool ObjectType::operator!=(const string& name)
{
	return this->name.compare(name) != 0;
}

int ObjectType::ToInt()
{
	return id;
}

int ObjectType::ToInt() const
{
	return id;
}

string ObjectType::ToString()
{
	return name;
}

string ObjectType::ToString() const
{
	return name;
}

ObjectType::ObjectType(int id, string name) {
	this->id = id;
	this->name = name;
}