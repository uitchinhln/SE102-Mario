#include "GameMap.h"
#include "Transform.h"
#include "ColliableTileAdapter.h"
#include "MapProperties.h"
#include "Events.h"

CGameMap::CGameMap()
{
	this->width = this->height = this->tileHeight = this->tileWidth = 0;
}

CGameMap::CGameMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
}

Vec2 CGameMap::GetBound()
{
	return Vec2((float)this->width * (float)tileWidth, (float)this->height * (float)tileHeight);
}

shared_ptr<CTileSet> CGameMap::GetTileSetByTileID(int id)
{
	return floor_entry(tilesets, id).second;
}

shared_ptr<ColliableTile> CGameMap::GetTileByGID(int id)
{
	shared_ptr<CTileSet> tileset = GetTileSetByTileID(id);
	if (tileset) {
		return tileset->GetColliableTile(id);
	}
	return nullptr;
}

Vec2 CGameMap::GetTileSize()
{
	return Vec2((float)tileWidth, (float)tileHeight);
}

void CGameMap::AddTileSet(int firstgid, shared_ptr<CTileSet> tileSet)
{
	this->tilesets[firstgid] = tileSet;
}

void CGameMap::AddLayer(shared_ptr<CLayer> layer)
{
	this->layers.push_back(layer);
}

void CGameMap::Update()
{
}

void CGameMap::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(backgroundColor);
	Transform trans = Transform();

	int col = (int) (this->camera->Position.x / tileWidth);
	int row = (int) (this->camera->Position.y / tileHeight);

	if (col > 0) col--;
	if (row > 0) row--;

	Vec2 camSize = Vec2(this->camera->GetCamSize().x / tileWidth, this->camera->GetCamSize().y / tileHeight);
	Vec2 camPos = camera->Position;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = col; i < camSize.x + col + 2; i++) {
		for (int j = row; j < camSize.y + row + 2; j++) {

			int x = i * tileWidth - camPos.x;
			int y = j * tileHeight - camPos.y;

			/*for (shared_ptr<CLayer> layer : layers) {
				if (layer->Hidden) continue;
				int id = layer->GetTileID(i, j);
				this->GetTileSetByTileID(id)->Draw(id, x, y, trans);
			}*/
		}
	}
	auto finish = std::chrono::high_resolution_clock::now();
	DebugOut(L"Render: \t%d\n", std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
}

bool CGameMap::IsTileObjectSupport()
{
	return this->tileObjectSupport;
}

shared_ptr<CGameMap> CGameMap::FromTMX(string filePath, string fileName)
{
	string fullPath = filePath + "/" + fileName;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		shared_ptr<CGameMap> gameMap = make_shared<CGameMap>();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		if (root->Attribute("backgroundcolor")) {
			string hexColor = root->Attribute("backgroundcolor");
			hexColor.replace(0, 1, "");
			unsigned int hex = stoul(hexColor, nullptr, 16);
			int a = (hex >> 24) & 255 | 255 & (hexColor.length() <= 6 ? 0xff : 0x00);
			gameMap->backgroundColor = D3DCOLOR_ARGB(a, (hex >> 16) & 255, (hex >> 8) & 255, hex & 255);
		}

		//Load custom properties
		TiXmlElement* properties = root->FirstChildElement("properties");
		MapProperties mapProps = MapProperties(properties);
		gameMap->tileObjectSupport = mapProps.GetBool("TileObjectSupport", true);

		//Load tileset
		gameMap->tilesets.clear();
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			shared_ptr<CTileSet> tileSet = make_shared<CTileSet>(node, filePath);
			gameMap->tilesets[tileSet->GetFirstGID()] = tileSet;
		}

		//Load layer
		gameMap->layers.clear();
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			shared_ptr<CLayer> layer = make_shared<CLayer>(node, gameMap);
			gameMap->AddLayer(layer);
		}

		for (TiXmlElement* node = root->FirstChildElement("objectgroup"); node != nullptr; node = node->NextSiblingElement("objectgroup")) {
			for (TiXmlElement* object = node->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object")) {
				Vec2 fixPos;
				Vec2 size = VECTOR_0;
				object->QueryFloatAttribute("x", &fixPos.x);
				object->QueryFloatAttribute("y", &fixPos.y);
				object->QueryFloatAttribute("width", &size.x);
				object->QueryFloatAttribute("height", &size.y);
				const char* type = object->Attribute("name");

				TiXmlElement* nodeProperties = object->FirstChildElement("properties");
				MapProperties props = MapProperties(nodeProperties);

				__raise (*Events::GetInstance()).ObjectLoadEvent(type, fixPos, size, props);
			}
		}

		return gameMap;
	}

	throw "Load map that bai!!";
}

CGameMap::~CGameMap()
{
	layers.clear();
	tilesets.clear();
}
