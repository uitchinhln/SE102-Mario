#pragma once
#include "Dialog.h"
#include "Text.h"
#include "Transform.h"

class LifeDialog :
	public Dialog
{
public:
	LifeDialog(long timeout);
	virtual void Update();
	virtual void Render(D3DXCOLOR overlay);
protected:
	Text life, world;

	long timer = 0;

	Transform trans;
};

