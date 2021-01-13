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
	this->texture = CGame::GetInstance()->GetGraphic().CreateTextureFromFile(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
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
	this->texture = CGame::GetInstance()->GetGraphic().CreateTextureFromFile(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
}

int CTileSet::GetFirstGID()
{
	return firstgid;
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
	texture->Release();
}
