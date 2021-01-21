#pragma once
#include "Utils.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include "Direction.h"

enum class CameraMode {
	TRACKING,
	AUTOSCROLL
};

class CameraRegion
{
protected:
	int id;

	RectF boundary{ -9999, -9999, 99999, 99999 };

	RectF inside{ -9999, -9999, 9999, 9999 };

	CameraMode mode = CameraMode::TRACKING;

	Vec2 velocity = VECTOR_0;

public:
	CameraRegion();

	CameraRegion(TiXmlElement* config);

	void SetID(int value);
	int GetID();

	void SetBoundary(RectF value);
	RectF GetBoundary();

	void SetCameraMode(CameraMode value);
	CameraMode GetCameraMode();

	void SetInsideBoundary(RectF value);
	RectF GetInsideBoundary();

	void SetVelocity(Vec2 value);
	Vec2 GetVelocity();
	
};

