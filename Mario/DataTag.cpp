#include "DataTag.h"

DataTag::DataTag(string tagName)
{
	this->tagName = tagName;
}

bool DataTag::HasNoTag()
{
	return contents.size() == 0;
}

bool DataTag::HasKey(string tagName)
{
	return contents.size() > 0 && contents.find(tagName) != contents.end();
}

void DataTag::RemoveTag(string name)
{
	contents.erase(name);
}

bool DataTag::GetBool(string tagName, bool default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<bool>(contents.at(tagName));
}

void DataTag::SetBool(string tagName, bool value)
{
	contents[tagName] = value;
}

int DataTag::GetInt(string tagName, int default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<int>(contents.at(tagName));
}

void DataTag::SetInt(string tagName, int value)
{
	contents[tagName] = value;
}

long DataTag::GetLong(string tagName, long default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<long>(contents.at(tagName));
}

void DataTag::SetLong(string tagName, long value)
{
	contents[tagName] = value;
}

double DataTag::GetDouble(string tagName, double default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<double>(contents.at(tagName));
}

void DataTag::SetDouble(string tagName, double value)
{
	contents[tagName] = value;
}

float DataTag::GetFloat(string tagName, float default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<float>(contents.at(tagName));
}

void DataTag::SetFloat(string tagName, float value)
{
	contents[tagName] = value;
}

string DataTag::GetString(string tagName, string default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<string>(contents.at(tagName));
}

void DataTag::SetString(string tagName, string value)
{
	contents[tagName] = value;
}

shared_ptr<DataTag> DataTag::GetDataTag(string tagName, shared_ptr<DataTag> default)
{
	if (!HasKey(tagName)) return default;
	return any_cast<shared_ptr<DataTag>>(contents.at(tagName));
}

void DataTag::SetDataTag(shared_ptr<DataTag> value)
{
	if (value == nullptr)
		throw "Value cannot be null!!!";
	if (&(*value) == this)
		throw "Loop detected!!!";
	if (&(*value->GetDataTag(tagName)) == this)
		throw "Loop detected!!!";

	contents[value->tagName] = value;
}
