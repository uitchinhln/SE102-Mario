#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Utils.h"
#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 1024

#define DIRECTINPUT_VERSION 0x0800

class KeyboardProcessor
{
	LPDIRECTINPUT8       di;								// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;								// The keyboard device 

	BYTE  keyStates[256];									// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

public:
	KeyboardProcessor();

	void InitKeyboard(HWND hWnd);

	void ProcessKeyboard();

	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);

	~KeyboardProcessor();
};
#endif
