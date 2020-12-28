#include "GameGraphic.h"
#include "Viewport.h"

void GameGraphic::Init(D3DPRESENT_PARAMETERS d3dpp, HWND hwnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

	d3ddv->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d3ddv->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	d3ddv->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

Vec2 GameGraphic::GetSceneSize()
{
	D3DVIEWPORT9 viewport;
	d3ddv->GetViewport(&viewport);
	return Vec2((float)viewport.Width, (float)viewport.Height);
}

void GameGraphic::Clear(D3DCOLOR color)
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void GameGraphic::Draw(float x, float y, D3DXVECTOR3 pivot, LPDIRECT3DTEXTURE9 texture, RECT r, Transform& transform, D3DCOLOR overlay)
{
	RECT viewport;
	this->d3ddv->GetScissorRect(&viewport);

	x += viewport.left;
	y += viewport.top;

	float spriteW = x + r.right - r.left;
	float spriteH = y + r.bottom - r.top;

	if (x > viewport.right || y > viewport.bottom || spriteW < viewport.left || spriteH < viewport.top) return;

	D3DXVECTOR3 p(x, y, 0);

	if (transform.Rotation == 0 && transform.Scale == Vec2(1.0f, 1.0f)) {
		spriteHandler->Draw(texture, &r, &pivot, &p, overlay);
	}
	else 
	{
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
	d3ddv->SetScissorRect(&viewport->GetScissorRect());
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
