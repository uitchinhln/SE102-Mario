#include "GameGraphic.h"

void GameGraphic::Init(D3DPRESENT_PARAMETERS d3dpp, HWND hwnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	d3ddv->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	d3ddv->GetViewport(&pViewport);

	d3ddv->GetScissorRect(&rScissorRect);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

Vec2 GameGraphic::GetSceneSize()
{
	return Vec2(pViewport.Width, pViewport.Height);
}

void GameGraphic::Clear(D3DCOLOR color, bool clearAll)
{
	if (clearAll) {
		D3DVIEWPORT9 old;
		d3ddv->GetViewport(&old);
		d3ddv->SetViewport(&pViewport);
		d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
		d3ddv->SetViewport(&old);
	}
	else {
		d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
	}
}

void GameGraphic::Draw(float x, float y, D3DXVECTOR3 pivot, LPDIRECT3DTEXTURE9 texture, RECT r, Transform& transform, D3DCOLOR overlay)
{
	float spriteW = x + r.right - r.left;
	float spriteH = y + r.bottom - r.top;
	D3DVIEWPORT9 viewport;
	this->d3ddv->GetViewport(&viewport);

	if (x > viewport.X + viewport.Width || y > viewport.Y + viewport.Height || spriteW < viewport.X || spriteH < viewport.Y) return;

	x -= viewport.X;
	y -= viewport.Y;

	D3DXVECTOR3 p(x, y, 0);

	if (transform.Rotation == 0 && transform.Scale == Vec2(1.0f, 1.0f)) {
		spriteHandler->Draw(texture, &r, &pivot, &p, overlay);
	}
	else {
		D3DXMATRIX oldMatrix, newMatrix;
		spriteHandler->GetTransform(&oldMatrix);

		Vec2 transformCenter = Vec2(x + (r.right - r.left) / 2, y + (r.bottom - r.top) / 2);

		D3DXMatrixTransformation2D(&newMatrix, &transformCenter, 0, &transform.Scale,
			&transformCenter, transform.Rotation, &VECTOR_0);

		spriteHandler->SetTransform(&newMatrix);
		spriteHandler->Draw(texture, &r, &pivot, &p, overlay);
		spriteHandler->SetTransform(&oldMatrix);
	}
}

LPDIRECT3DTEXTURE9 GameGraphic::CreateTextureFromFile(LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	HRESULT result = D3DXGetImageInfoFromFile(texturePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", texturePath);
		return NULL;
	}

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,			// Transparent color
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed. File: %s\n", texturePath);
		return NULL;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
	return texture;
}

void GameGraphic::SetViewport(shared_ptr<Viewport> viewport)
{
	ClipScene();
	d3ddv->SetViewport(&(viewport->GetD3DViewport()));
	d3ddv->SetScissorRect(&(viewport->GetScissorRect()));
}

void GameGraphic::ClipScene()
{
	spriteHandler->End();
	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
}

GameGraphic::~GameGraphic()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}
