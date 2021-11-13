#include "GameGraphic.h"
#include "Viewport.h"
#include "TextureManager.h"

void GameGraphic::Init(HWND hWnd)
{
	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right + 1;
	backBufferHeight = r.bottom + 1;

	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_REFERENCE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", __FILE__, __LINE__);
		return;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", __FILE__, __LINE__);
		return;
	}

	// create the render target view
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	pBackBuffer->Release();
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", __FILE__, __LINE__);
		return;
	}

	// set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice->RSSetViewports(1, &viewPort);


	// create the sprite object to handle sprite drawing 
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10CreateSprite has failed %s %d", __FILE__, __LINE__);
		return;
	}

	D3DXMATRIX matProjection;

	// Create the projection matrix using the values in the viewport
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.TopLeftY,
		(float)viewPort.Height,
		0.1f,
		10);
	hr = spriteObject->SetProjectionTransform(&matProjection);


	// Initialize the blend state for alpha drawing
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	pD3DDevice->CreateBlendState(&StateDesc, &this->pBlendStateAlpha);

	ID3D10RasterizerState* g_pRasterState;
	D3D10_RASTERIZER_DESC rasterizerState;

	rasterizerState.FillMode = D3D10_FILL_SOLID;
	rasterizerState.CullMode = D3D10_CULL_NONE;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = true;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;

	pD3DDevice->CreateRasterizerState(&rasterizerState, &g_pRasterState);

	pD3DDevice->RSSetState(g_pRasterState);

	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

void GameGraphic::Clear(D3DXCOLOR color)
{
	LPTEXTURE white = TextureManager::GetInstance()->Get("tex-white");
	RECT r;
	r.left = r.top = 0;
	r.right = white->getWidth();
	r.bottom = white->getHeight();

	RECT viewport;
	UINT id = 1;
	this->pD3DDevice->RSGetScissorRects(&id, &viewport);

	LONG width = viewport.right - viewport.left;
	LONG height = viewport.bottom - viewport.top;

	D3DXVECTOR3 pivot(0, 0, 0);
	Transform trans;

	for (int i = 0; i < width / white->getWidth() + 2; i++) {
		for (int j = 0; j < height / white->getHeight() + 2; j++) {
			Draw(i * white->getWidth(), j * white->getHeight(), pivot, white, r, trans, color);
		}
	}
}

void GameGraphic::Draw(float x, float y, D3DXVECTOR3 pivot, LPTEXTURE texture, RECT r, Transform& transform, D3DXCOLOR overlay)
{
	if (texture == NULL) return;

	RECT viewport;
	UINT id = 1;
	this->pD3DDevice->RSGetScissorRects(&id, &viewport);

	x += viewport.left;
	y += viewport.top;

	int spriteWidth = 0;
	int spriteHeight = 0;

	D3DX10_SPRITE sprite;

	// Set the sprite’s shader resource view
	sprite.pTexture = texture->getShaderResourceView();

	sprite.TexCoord.x = r.left / (float)texture->getWidth();
	sprite.TexCoord.y = r.top / (float)texture->getHeight();

	spriteWidth = r.right - r.left;
	spriteHeight = r.bottom - r.top;

	sprite.TexSize.x = spriteWidth / (float)texture->getWidth();
	sprite.TexSize.y = spriteHeight / (float)texture->getHeight();

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;

	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = ToFloatColor(overlay);


	//
	// Build the rendering matrix based on sprite location 
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	pivot.x = pivot.x == 0 ? spriteWidth / 2 : pivot.x;
	pivot.y = pivot.y == 0 ? spriteHeight / 2 : pivot.y;

	FLOAT rx = x + (spriteWidth / 2 - pivot.x) * transform.Scale.x;
	FLOAT ry = (backBufferHeight - y) - (spriteHeight / 2 - pivot.y) * transform.Scale.y;

	// Create the translation matrix
	D3DXMatrixTranslation(&matTranslation, rx, ry, 0.1f);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f 
	D3DXMATRIX matScaling;
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth * transform.Scale.x, (FLOAT)spriteHeight * transform.Scale.y, 1.0f);

	// Setting the sprite’s position and size
	sprite.matWorld = (matScaling * matTranslation);

	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

LPTEXTURE GameGraphic::CreateTextureFromFile(LPCWSTR texturePath)
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Retrieve image information first 
	D3DX10_IMAGE_INFO imageInfo;
	HRESULT hr = D3DX10GetImageInfoFromFile(texturePath, NULL, &imageInfo, NULL);
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10GetImageInfoFromFile failed for  file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	D3DX10_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
	info.Width = imageInfo.Width;
	info.Height = imageInfo.Height;
	info.Depth = imageInfo.Depth;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;
	info.Usage = D3D10_USAGE_DEFAULT;
	info.BindFlags = D3DX10_DEFAULT;
	info.CpuAccessFlags = D3DX10_DEFAULT;
	info.MiscFlags = D3DX10_DEFAULT;
	info.Format = imageInfo.Format;
	info.Filter = D3DX10_FILTER_NONE;
	info.MipFilter = D3DX10_DEFAULT;
	info.pSrcInfo = &imageInfo;

	// Loads the texture into a temporary ID3D10Resource object
	hr = D3DX10CreateTextureFromFile(pD3DDevice,
		texturePath,
		&info,
		NULL,
		&pD3D10Resource,
		NULL);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
	pD3D10Resource->Release();

	if (!tex) {
		DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return NULL;
	}

	//
	// Create the Share Resource View for this texture 
	// 	   
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	// Clear out the shader resource view description structure
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	// Set the texture format
	SRVDesc.Format = desc.Format;

	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

	pD3DDevice->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

	D3DX10FilterTexture(tex, 0, D3DX10_FILTER_MIRROR);

	DebugOut(L"[INFO] Texture loaded Ok from file: %s \n", texturePath);

	return new Texture(tex, gSpriteTextureRV);
}

void GameGraphic::SetViewport(shared_ptr<Viewport> viewport)
{
	ClipScene();
	this->pD3DDevice->RSSetScissorRects(1, &viewport->GetScissorRect());
}

void GameGraphic::ClipScene()
{
	spriteObject->End();
	spriteObject->Begin(D3DX10_SPRITE_SORT_TEXTURE);
}

GameGraphic::~GameGraphic()
{
	if (pD3DDevice != NULL) pD3DDevice->Release();
	if (pSwapChain != NULL) pSwapChain->Release();
	if (pRenderTargetView != NULL) pRenderTargetView->Release();
	if (pBlendStateAlpha != NULL) pBlendStateAlpha->Release();
	if (spriteObject != NULL) spriteObject->Release();
}
