#pragma once
#include "Utils.h"
#include "GameProperty.h"


class GameWindow
{
	HWND hWnd;

	Vec2 windowSize;

	LPCWSTR windowClassName;

	LPCWSTR windowTitle;

	LPCWSTR iconPath;

public:
	GameWindow(GameProperties properties);

	void CreateGameWindow();

	HWND GetWindowHandler();

	Vec2 GetWindowSize();

	void SetTitle(LPCWSTR title);

	void SetIcon(LPCWSTR iconPath);

private:
	static HINSTANCE hInstance;
	static LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

