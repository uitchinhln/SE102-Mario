#include "TileSet.h"
#include "Game.h"
#include "TextureManager.h"

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
}

int CTileSet::GetFirstGID()
{
	return firstgid;
}

void CTileSet::Draw(int gid, float x, float y, D3DCOLOR overlay)
{
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->Draw(x, y, texture, r, Vec2(1, 1), 0, overlay);
}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}