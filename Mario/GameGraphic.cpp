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

	OutputDebugString(L"[INFO] InitGame done;\n");
}

Vec2 GameGraphic::GetViewportSize()
{
	D3DVIEWPORT9 viewport;
	this->d3ddv->GetViewport(&viewport);
	return Vec2(viewport.Width, viewport.Height);
}

void GameGraphic::Clear(D3DCOLOR color)
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void GameGraphic::Draw(float x, float y, D3DXVECTOR3 pivot, LPDIRECT3DTEXTURE9 texture, RECT r, Transform& transform, D3DCOLOR overlay)
{
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

GameGraphic::~GameGraphic()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}
