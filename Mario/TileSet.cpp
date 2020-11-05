#include "TileSet.h"
#include "Game.h"
#include "TextureManager.h"
#include "Events.h"
#include "MapProperties.h"

CTileSet::CTileSet(int firstgid, Vec2 tileSize, int tileCount, int columns, string imgPath)
{
	this->firstgid = firstgid;
	this->tileSize = Vec2(tileSize.x, tileSize.y);
	this->tileCount = tileCount;
	this->columns = columns;
	this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
}

CTileSet::CTileSet(TiXmlElement* data, string xmlPath)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);

	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = xmlPath + "/" + imgDom->Attribute("source");
	this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));

	for (TiXmlElement* node = data->FirstChildElement("tile"); node != nullptr ; node = node->NextSiblingElement("tile"))
	{
		int id = 0; node->QueryIntAttribute("id", &id);
		TiXmlElement* objects = node->FirstChildElement("objectgroup");
		for (TiXmlElement* object = objects->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object")) {
			if (object->Attribute("height") != NULL) {
				shared_ptr<ColliableTile> tile;
				const char* type = object->Attribute("name");

				if (type) {
					TiXmlElement* propsEle = object->FirstChildElement("properties");
					MapProperties props = MapProperties(propsEle);

					__raise (*Events::GetInstance()).ColliableTilePreLoadEvent(type, id, tile, props);

					if (!tile) tile = make_shared<ColliableTile>(id);
				}
				else {
					tile = make_shared<ColliableTile>(id);
				}		

				RectF rect;
				object->QueryFloatAttribute("x", &rect.left);
				object->QueryFloatAttribute("y", &rect.top);
				object->QueryFloatAttribute("width", &rect.right);
				object->QueryFloatAttribute("height", &rect.bottom);

				rect.right = rect.right + rect.left;
				rect.bottom = rect.top + rect.bottom;

				tile->SetHitBox(rect);

				this->colliableTiles[id+firstgid] = tile;
			}
		}
	}
}

int CTileSet::GetFirstGID()
{
	return firstgid;
}

shared_ptr<ColliableTile> CTileSet::GetColliableTile(int id)
{
	if (id < firstgid) return nullptr;
	if (colliableTiles.find(id) == colliableTiles.end()) return nullptr;
	return colliableTiles[id];
}

void CTileSet::Draw(int gid, float x, float y, Transform& transform, D3DCOLOR overlay)
{
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->GetGraphic().Draw(x, y, D3DXVECTOR3(0, 0, 0), texture, r, transform, overlay);
}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}
