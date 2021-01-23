#pragma once
#include "Utils.h"

class MarioKeyboard
{
protected:
	BYTE keyState[255];

	bool globalEnable = true;

public:
	MarioKeyboard();

	virtual void PressKey(int keycode);
	virtual void ReleaseKey(int keycode);

	virtual bool IsGlobalKeyBoardEnable();
	virtual void SetGlobalKeyBoardEnable(bool value);
	
	virtual bool IsKeyUp(int keycode);
	virtual bool IsKeyDown(int keycode);
};

