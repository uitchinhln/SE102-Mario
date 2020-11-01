#pragma once
#include "Utils.h"
#include <any>

class DataTag
{
	string tagName;
	unordered_map<string, any> contents;
public:
	DataTag(string tagName);

	bool HasNoTag();

	bool HasKey(string tagName);

	void RemoveTag(string name);

	bool GetBool(string tagName, bool default = false);
	void SetBool(string tagName, bool value);

	int GetInt(string tagName, int default = 0);
	void SetInt(string tagName, int value);

	long GetLong(string tagName, long default = 0);
	void SetLong(string tagName, long value);

	double GetDouble(string tagName, double default = 0);
	void SetDouble(string tagName, double value);

	float GetFloat(string tagName, float default = 0);
	void SetFloat(string tagName, float value);

	string GetString(string tagName, string default = "");
	void SetString(string tagName, string value);

	shared_ptr<DataTag> GetDataTag(string tagName, shared_ptr<DataTag> default = nullptr);
	void SetDataTag(shared_ptr<DataTag> value);
};

