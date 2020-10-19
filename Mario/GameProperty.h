#pragma once
#include <string>
#include <any>
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

class CGameProperties
{
public:
	CGameProperties();

	int TickRate = 120;

	int ScreenWidth = 800;
	int ScreenHeight = 600;

	LPCWSTR MainWindowTitle = L"Mario Game";
	LPCWSTR WindowClassName = L"Mario Game Class";

	D3DPRESENT_PARAMETERS d3dpp;

private:
	unordered_map<string, any> properties;
};

typedef CGameProperties* GameProperties;

