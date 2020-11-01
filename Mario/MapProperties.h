#pragma once
#include "Utils.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include "DataTag.h"

class MapProperties
{
public:
	MapProperties();
	MapProperties(TiXmlElement* data);

	virtual bool HasProperty(string name);

	virtual string GetText(string name, string def = "");
	virtual bool GetBool(string name, bool def = true);
	virtual int GetInt(string name, int def = 0);
	virtual float GetFloat(string name, float def = 0.0f);

protected:
	shared_ptr<DataTag> dataContainer = make_shared<DataTag>("Root");
};

