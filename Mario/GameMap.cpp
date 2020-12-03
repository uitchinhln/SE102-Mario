#include "GameMap.h"
#include "Transform.h"
#include "MapProperties.h"
#include "Events.h"
#include "Game.h"

CGameMap::CGameMap()
{
	this->width = this->height = this->tileHeight = this->tileWidth = 0;
}

Vec2 CGameMap::GetBound()
{
	return Vec2((float)this->width * (float)tileWidth, (float)this->height * (float)tileHeight);
}

Vec2 CGameMap::GetTileSize()
{
	return Vec2((float)tileWidth, (float)tileHeight);
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
	//auto start = std::chrono::high_resolution_clock::now();
	CGame::GetInstance()->GetGraphic().Clear(backgroundColor);
	Transform trans = Transform();

	int col = (int) (this->camera->Position.x / tileWidth);
	int row = (int) (this->camera->Position.y / tileHeight);

	if (col > 0) col--;
	if (row > 0) row--;

	Vec2 camSize = Vec2(this->camera->GetCamSize().x / tileWidth, this->camera->GetCamSize().y / tileHeight);
	Vec2 camPos = camera->Position;

	int firstGID = tileSet->GetFirstGID();

	for (CLayer* layer : ptr_layers) {
		if (!layer->Visible()) continue;

		int** tiles = layer->GetTiles();

		for (int i = col; i < camSize.x + col + 2; i++) {
			if (i < 0 || i >= width) continue;

			for (int j = row; j < camSize.y + row + 2; j++) {
				if (j < 0 || j >= height) continue;

				int id = tiles[i][j];
				if (id < firstGID) continue;

				int x = i * tileWidth - camPos.x;
				int y = j * tileHeight - camPos.y;

				tileSet->Draw(id, x, y, trans);
			}
		}
	}
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Render: \t%d\n", std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
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

		//Load tileset
		gameMap->tileSet = make_shared<CTileSet>(root->FirstChildElement("tileset"), filePath);

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			shared_ptr<CLayer> layer = make_shared<CLayer>(node, gameMap);
			gameMap->AddLayer(layer);
			gameMap->ptr_layers.push_back(&*layer);
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
}
