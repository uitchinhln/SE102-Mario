#include "GameWindow.h"

HINSTANCE GameWindow::hInstance = nullptr;

GameWindow::GameWindow(GameProperties properties)
{
	hInstance = GetModuleHandle(nullptr);

	windowSize = Vec2((float)properties->ScreenWidth, (float)properties->ScreenHeight);

	windowClassName = properties->WindowClassName;
	windowTitle = properties->MainWindowTitle;

	hWnd = nullptr;

	iconPath = nullptr;
}

void GameWindow::CreateGameWindow()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	this->hWnd =
		CreateWindow(
			windowClassName,
			windowTitle,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			(GetSystemMetrics(SM_CXSCREEN) - (int)windowSize.x) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - (int)windowSize.y) / 2,
			(int)windowSize.x,
			(int)windowSize.y,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!this->hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return;
	}

	ShowWindow(this->hWnd, SW_SHOWNORMAL);
	UpdateWindow(this->hWnd);
}

HWND GameWindow::GetWindowHandler()
{
    return this->hWnd;
}

Vec2 GameWindow::GetWindowSize()
{
	return windowSize;
}

void GameWindow::SetTitle(LPCWSTR title)
{
	this->windowClassName = this->windowTitle = title;
}

void GameWindow::SetIcon(LPCWSTR iconPath)
{
	this->iconPath = iconPath;
}

LRESULT GameWindow::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
