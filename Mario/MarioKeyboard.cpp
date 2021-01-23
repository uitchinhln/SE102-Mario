#include "MarioKeyboard.h"
#include "MarioGame.h"

MarioKeyboard::MarioKeyboard()
{
	for (int i = 0; i < 255; i++)
	{
		keyState[i] = 0;
	}
}

void MarioKeyboard::PressKey(int keycode)
{
	keyState[keycode] = 1;
}

void MarioKeyboard::ReleaseKey(int keycode)
{
	keyState[keycode] = 0;
}

bool MarioKeyboard::IsGlobalKeyBoardEnable()
{
	return globalEnable;
}

void MarioKeyboard::SetGlobalKeyBoardEnable(bool value)
{
	this->globalEnable = value;
}

bool MarioKeyboard::IsKeyUp(int keycode)
{
	bool localKeyUp = keyState[keycode] == 0;
	if (globalEnable) {
		return localKeyUp && MarioGame::GetInstance()->GetKeyBoard().IsKeyUp(keycode);
	}
	return localKeyUp;
}

bool MarioKeyboard::IsKeyDown(int keycode)
{
	bool localKeyUp = keyState[keycode] > 0;
	if (globalEnable) {
		return localKeyUp || MarioGame::GetInstance()->GetKeyBoard().IsKeyDown(keycode);
	}
	return localKeyUp;
}
