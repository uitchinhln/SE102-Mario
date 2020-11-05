#include "MapProperties.h"

MapProperties::MapProperties()
{
}

MapProperties::MapProperties(TiXmlElement* data)
{
	if (data == nullptr) return;
	for (TiXmlElement* prop = data->FirstChildElement("property"); prop != nullptr; prop = prop->NextSiblingElement("property")) {
		string type = "text";
		if (prop->Attribute("type")) {
			type = prop->Attribute("type");
		}

		if (type.compare("text") == 0) {
			string name = prop->Attribute("name");
			string value = prop->Attribute("value");
			dataContainer->SetString(name, value);
		}

		if (type.compare("bool") == 0) {
			string name = prop->Attribute("name");
			bool value = false;
			prop->QueryBoolAttribute("value", &value);
			dataContainer->SetBool(name, value);
		}

		if (type.compare("int") == 0) {
			string name = prop->Attribute("name");
			int value = 0;
			prop->QueryIntAttribute("value", &value);
			dataContainer->SetInt(name, value);
		}

		if (type.compare("float") == 0) {
			string name = prop->Attribute("name");
			float value = 0.0f;
			prop->QueryFloatAttribute("value", &value);
			dataContainer->SetFloat(name, value);
		}
	}
}

bool MapProperties::HasProperty(string name)
{
	return dataContainer->HasKey(name);
}

string MapProperties::GetText(string name, string def)
{
	return dataContainer->GetString(name, def);
}

bool MapProperties::GetBool(string name, bool def)
{
	return dataContainer->GetBool(name, def);
}

int MapProperties::GetInt(string name, int def)
{
	return dataContainer->GetInt(name, def);
}

float MapProperties::GetFloat(string name, float def)
{
	return dataContainer->GetFloat(name, def);
}
