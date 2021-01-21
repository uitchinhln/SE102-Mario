#include "CameraRegion.h"

CameraRegion::CameraRegion()
{
}

CameraRegion::CameraRegion(TiXmlElement* config)
{
	config->QueryIntAttribute("id", &id);

	if (config->Attribute("left") != nullptr) {
		config->QueryFloatAttribute("left", &boundary.left);
	}
	if (config->Attribute("top") != nullptr) {
		config->QueryFloatAttribute("top", &boundary.top);
	}
	if (config->Attribute("right") != nullptr) {
		config->QueryFloatAttribute("right", &boundary.right);
	}
	if (config->Attribute("bottom") != nullptr) {
		config->QueryFloatAttribute("bottom", &boundary.bottom);
	}

	if (config->NoChildren()) return;

	TiXmlElement* modeNode = config->FirstChildElement("CameraMode");
	if (modeNode == NULL) return;

	if (strcmp(modeNode->Attribute("mode"), "AutoScroll") == 0) {
		this->mode = CameraMode::AUTOSCROLL;
	}
	else {
		this->mode = CameraMode::TRACKING;
	}


	if (modeNode->Attribute("velocityX") != nullptr) {
		modeNode->QueryFloatAttribute("velocityX", &velocity.x);
	}
	if (modeNode->Attribute("velocityY") != nullptr) {
		modeNode->QueryFloatAttribute("velocityY", &velocity.y);
	}

	if (modeNode->NoChildren()) return;

	TiXmlElement* insideBound = modeNode->FirstChildElement("InsideBoundary");
	if (insideBound == NULL) return;


	if (insideBound->Attribute("left") != nullptr) {
		insideBound->QueryFloatAttribute("left", &inside.left);
	}
	if (insideBound->Attribute("top") != nullptr) {
		insideBound->QueryFloatAttribute("top", &inside.top);
	}
	if (insideBound->Attribute("right") != nullptr) {
		insideBound->QueryFloatAttribute("right", &inside.right);
	}
	if (insideBound->Attribute("bottom") != nullptr) {
		insideBound->QueryFloatAttribute("bottom", &inside.bottom);
	}
}

