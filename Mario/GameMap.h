#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "Layer.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include "Camera.h"
#include "IColliable.h"

class CGameMap
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	D3DCOLOR backgroundColor = D3DCOLOR_XRGB(181, 235, 242);

	shared_ptr<Camera> camera;

	shared_ptr<CTileSet> tileSet;

	vector<shared_ptr<CLayer>> layers;
	vector<CLayer*> ptr_layers;
	
public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);

	virtual shared_ptr<Camera> GetCamera() { return this->camera; }

	virtual void SetCamera(shared_ptr<Camera> camera) { this->camera = camera; }

	virtual Vec2 GetBound();

	virtual Vec2 GetTileSize();

	virtual void AddLayer(shared_ptr<CLayer> layer);

	virtual void Update();

	virtual void Render();

	static shared_ptr<CGameMap> FromTMX(string filePath, string fileName);
	~CGameMap();
};

typedef CGameMap* GameMap;

