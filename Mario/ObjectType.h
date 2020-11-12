#pragma once
#include "Utils.h"

#define DEFAULT_TILE_ID -51561
#define DEFAULT_TILE_NAME "Tile"

class ObjectType
{
public:
	static ObjectType CreateObjectType(int id, string name);

	bool operator==(const ObjectType& target);

	bool operator!=(const ObjectType& target);

	bool operator==(const int& name);

	bool operator!=(const int& name);

	bool operator==(const string& name);

	bool operator!=(const string& name);

	int ToInt();

	int ToInt() const;

	string ToString();

	string ToString() const;
private:
	ObjectType(int id, string name);

	int id;

	string name;
};

