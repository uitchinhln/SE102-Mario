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

	map<int, shared_ptr<CTileSet>> tilesets;
	vector<shared_ptr<CLayer>> layers;
	
public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);

	virtual shared_ptr<Camera> GetCamera() { return this->camera; }

	virtual void SetCamera(shared_ptr<Camera> camera) { this->camera = camera; }

	virtual Vec2 GetBound();

	virtual shared_ptr<CTileSet> GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, shared_ptr<CTileSet> tileSet);

	virtual void AddLayer(shared_ptr<CLayer> layer);

	virtual void Update();

	virtual void Render();

	virtual vector<shared_ptr<IColliable>> GetColliableTileAround(Vec2 absolutePosition, RectF boundingBox, Vec2 radius);

	static shared_ptr<CGameMap> FromTMX(string filePath, string fileName);
	~CGameMap();
};

typedef CGameMap* GameMap;

