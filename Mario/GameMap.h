#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "Layer.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include "Camera.h"

class CGameMap
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	D3DXCOLOR backgroundColor = D3DXCOLOR(181, 235, 242, 255);

	shared_ptr<Camera> camera;

	shared_ptr<CTileSet> tileSet;

	vector<shared_ptr<CLayer>> layers;
	vector<CLayer*> ptr_layers;
	
public:
	CGameMap();

	virtual shared_ptr<Camera> GetCamera() { return this->camera; }

	virtual void SetCamera(shared_ptr<Camera> camera) { this->camera = camera; }

	virtual Vec2 GetBound();

	virtual Vec2 GetTileSize();

	virtual void AddLayer(shared_ptr<CLayer> layer);

	virtual void Update();

	virtual void Render(D3DXCOLOR overlay);

	virtual void SetBackgroundColor(D3DXCOLOR color);

	static shared_ptr<CGameMap> FromTMX(string filePath);
	~CGameMap();
};

typedef CGameMap* GameMap;

