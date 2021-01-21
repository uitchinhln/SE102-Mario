#include "CameraRegion.h"

void CameraRegion::SetID(int value)
{
    this->id = value;
}

int CameraRegion::GetID()
{
    return this->id;
}

void CameraRegion::SetBoundary(RectF value)
{
    this->boundary = value;
}

RectF CameraRegion::GetBoundary()
{
    return this->boundary;
}

void CameraRegion::SetCameraMode(CameraMode value)
{
    this->mode = value;
}

CameraMode CameraRegion::GetCameraMode()
{
    return this->mode;
}

void CameraRegion::SetInsideBoundary(RectF value)
{
    this->inside = value;
}

RectF CameraRegion::GetInsideBoundary()
{
    return inside;
}

void CameraRegion::SetVelocity(Vec2 value)
{
    this->velocity = value;
}

Vec2 CameraRegion::GetVelocity()
{
    return this->velocity;
}