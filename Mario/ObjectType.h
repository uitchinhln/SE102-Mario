#pragma once
#include "Utils.h"

class ObjectType
{
public:
	static ObjectType CreateObjectType(string name);

	bool operator==(const ObjectType& target);

	bool operator!=(const ObjectType& target);

	bool operator==(const string& name);

	bool operator!=(const string& name);

	string ToString();
private:
	ObjectType(string name);

	string name;
};

