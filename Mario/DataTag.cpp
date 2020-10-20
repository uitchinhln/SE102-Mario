#include "DataTag.h"

DataTag::DataTag(string tagName)
{
	this->tagName = tagName;
}

bool DataTag::hasNoTag()
{
	return contents.size() == 0;
}

bool DataTag::hasKey(string tagName)
{
	return contents.size() > 0 && contents.find(tagName) != contents.end();
}

bool DataTag::getBool(string tagName, bool default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<bool>(contents.at(tagName));
}

void DataTag::setBool(string tagName, bool value)
{
	contents[tagName] = value;
}

int DataTag::getInt(string tagName, int default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<int>(contents.at(tagName));
}

void DataTag::setInt(string tagName, int value)
{
	contents[tagName] = value;
}

long DataTag::getLong(string tagName, long default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<long>(contents.at(tagName));
}

void DataTag::setLong(string tagName, long value)
{
	contents[tagName] = value;
}

double DataTag::getDouble(string tagName, double default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<double>(contents.at(tagName));
}

void DataTag::setDouble(string tagName, double value)
{
	contents[tagName] = value;
}

float DataTag::getFloat(string tagName, float default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<float>(contents.at(tagName));
}

void DataTag::setFloat(string tagName, float value)
{
	contents[tagName] = value;
}

string DataTag::getString(string tagName, string default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<string>(contents.at(tagName));
}

void DataTag::setString(string tagName, string value)
{
	contents[tagName] = value;
}

shared_ptr<DataTag> DataTag::getDataTag(string tagName, shared_ptr<DataTag> default)
{
	if (!hasKey(tagName)) return default;
	return any_cast<shared_ptr<DataTag>>(contents.at(tagName));
}

void DataTag::setDataTag(shared_ptr<DataTag> value)
{
	if (value == nullptr)
		throw "Value cannot be null!!!";
	if (&(*value) == this)
		throw "Loop detected!!!";
	if (&(*value->getDataTag(tagName)) == this)
		throw "Loop detected!!!";

	contents[value->tagName] = value;
}
