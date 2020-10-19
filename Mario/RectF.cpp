#include "RectF.h"

RectF::RectF()
{
	this->left = this->top = this->right = this->bottom = 0;
}

RectF::RectF(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}
