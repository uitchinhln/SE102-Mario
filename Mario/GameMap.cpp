#include "GameMap.h"
#include "Transform.h"
#include "ColliableTileAdapter.h"

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

Vec2 CGameMap::GetBound()
{
	return Vec2(this->width * tileWidth, this->height * tileHeight);
}

shared_ptr<CTileSet> CGameMap::GetTileSetByTileID(int id)
{
	return floor_entry(tilesets, id).second;
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
	int col = this->camera->Position.x / tileWidth;
	int row = this->camera->Position.y / tileHeight;

	if (col > 0) col--;
	if (row > 0) row--;

	Vec2 camSize = Vec2(this->camera->GetCamSize().x / tileWidth, this->camera->GetCamSize().y / tileHeight);

	for (int i = col; i < camSize.x + col + 2; i++) {
		for (int j = row; j < camSize.y + row + 2; j++) {

			int x = i * tileWidth - this->camera->Position.x;
			int y = j * tileHeight - this->camera->Position.y;

			for (shared_ptr<CLayer> layer : layers) {
				if (layer->Hidden) continue;
				int id = layer->GetTileID(i % width, j % height);
				this->GetTileSetByTileID(id)->Draw(id, x, y, Transform());
			}
		}
	}
}

vector<shared_ptr<IColliable>> CGameMap::GetColliableTileAround(Vec2 absolutePosition, RectF boundingBox, Vec2 radius)
{
	int col = trunc(absolutePosition.x / tileWidth);
	int row = trunc(absolutePosition.y / tileHeight);

	int col_end = trunc(col + (boundingBox.right - boundingBox.left) / tileWidth);
	int row_end = trunc(row + (boundingBox.bottom - boundingBox.top) / tileHeight);

	Vec2 r = Vec2((abs(radius.x) + tileWidth * 2) / tileWidth, (abs(radius.y) + tileHeight * 2) / tileHeight);

	//DebugOut(L"R (%f, %f)\n", r.x, r.y);

	vector<shared_ptr<IColliable>> result;
	int is = col - r.x < 0 ? 0 : col - r.x;
	int ie = col_end + r.x > width ? width : col_end + r.x;
	int js = row - r.y < 0 ? 0 : row - r.y;
	int je = row_end + r.y > height ? height : row_end + r.y;

	for (int i = is; i <= ie; i++) {
		for (int j = js; j <= je; j++) {

			if (i >= col && i <= col_end && j >= row && j <= row_end) continue;

			for (shared_ptr<CLayer> layer : layers) {
				int id = layer->GetTileID(i, j);
				//if (id == 107) DebugOut(L"id = %d\n", id);
				shared_ptr<CTileSet> tileset = GetTileSetByTileID(id);
				if (tileset) {
					shared_ptr<ColliableTile> tile = tileset->GetColliableTile(id);
					if (tile) {
						int x = i * tileWidth;
						int y = j * tileHeight;
						shared_ptr<ColliableTileAdapter> adapter = make_shared<ColliableTileAdapter>(tile, Vec2(x, y));
						result.push_back(adapter);
					}
				}
			}
		}
	}
	return result;
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

		//Load tileset
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			shared_ptr<CTileSet> tileSet = make_shared<CTileSet>(node, filePath);
			gameMap->tilesets[tileSet->GetFirstGID()] = shared_ptr<CTileSet>(tileSet);
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			shared_ptr<CLayer> layer = make_shared<CLayer>(node);
			gameMap->AddLayer(layer);
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
