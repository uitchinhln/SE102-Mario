#pragma once
#include "Utils.h"
#include <any>

class DataTag
{
	string tagName;
	unordered_map<string, any> contents;
public:
	DataTag(string tagName);

	bool hasNoTag();

	bool hasKey(string tagName);

	bool getBool(string tagName, bool default = false);
	void setBool(string tagName, bool value);

	int getInt(string tagName, int default = 0);
	void setInt(string tagName, int value);

	long getLong(string tagName, long default = 0);
	void setLong(string tagName, long value);

	double getDouble(string tagName, double default = 0);
	void setDouble(string tagName, double value);

	float getFloat(string tagName, float default = 0);
	void setFloat(string tagName, float value);

	string getString(string tagName, string default = "");
	void setString(string tagName, string value);

	shared_ptr<DataTag> getDataTag(string tagName, shared_ptr<DataTag> default = nullptr);
	void setDataTag(shared_ptr<DataTag> value);
};

