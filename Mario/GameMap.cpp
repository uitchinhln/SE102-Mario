#include "GameMap.h"

CGameMap::CGameMap()
{
	
}

CGameMap::CGameMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
}

TileSet CGameMap::GetTileSetByTileID(int id)
{
	return floor_entry(tilesets, id).second;
}

void CGameMap::AddTileSet(int firstgid, TileSet tileSet)
{
	this->tilesets[firstgid] = tileSet;
}

void CGameMap::AddLayer(Layer layer)
{
	this->layers.push_back(layer);
}

void CGameMap::Update(int dt)
{
}

void CGameMap::Render()
{
	int col = this->camera->GetPosition().x / tileWidth;
	int row = this->camera->GetPosition().y / tileHeight;

	if (col > 0) col--;
	if (row > 0) row--;

	Vec2 camSize = Vec2(this->camera->GetCamSize().x / tileWidth, this->camera->GetCamSize().y / tileHeight);

	for (int i = col; i < camSize.x + col + 2; i++) {
		for (int j = row; j < camSize.y + row + 2; j++) {
			int x = i * tileWidth - this->camera->GetPosition().x;
			int y = j * tileHeight - this->camera->GetPosition().y;
			for (Layer layer : layers) {
				int id = layer->GetTileID(i % width, j % height);
				this->GetTileSetByTileID(id)->Draw(id, x, y);
			}
		}
	}
}

CGameMap* CGameMap::FromTMX(string filePath, string fileName)
{
	string fullPath = filePath + "/" + fileName;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		GameMap gameMap = new CGameMap();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);
			gameMap->tilesets[tileSet->GetFirstGID()] = tileSet;
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			Layer layer = new CLayer(node);
			gameMap->AddLayer(layer);
		}

		return gameMap;
	}

	return nullptr;
}

CGameMap::~CGameMap()
{
	
}
