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

	Camera* camera;

	map<int, TileSet> tilesets;
	vector<Layer> layers;
	
public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);

	virtual Camera* GetCamera() { return this->camera; }

	virtual void SetCamera(Camera* camera) { this->camera = camera; }

	virtual Vec2 GetBound();

	virtual TileSet GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, TileSet tileSet);

	virtual void AddLayer(Layer layer);

	virtual void Update(int dt);

	virtual void Render();

	static CGameMap* FromTMX(string filePath, string fileName);
	~CGameMap();
};

typedef CGameMap* GameMap;

