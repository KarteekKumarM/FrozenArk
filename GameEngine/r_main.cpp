#include "w_main.h"
#include "console.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

static IDXGISwapChain* s_dxgiSwapChain;
static ID3D11Device* s_d3dDevice;
static ID3D11DeviceContext* s_d3dDeviceContext;
static ID3D11Texture2D* s_BackBuffer;
static ID3D11RenderTargetView* s_d3dBackBufferRenderTargetView;
static ID3D11Texture2D* s_depthStencilBuffer;
static ID3D11DepthStencilState* s_depthStencilState;
static ID3D11DepthStencilView* s_depthStencilView;
static ID3D11BlendState* s_alphaEnableBlendingState;
static ID3D11RasterizerState* s_d3dRasterizerState;

void R_SetupSwapChain(HWND hWnd, UINT screenWidth, UINT screenHeight, bool shouldFullscreen)
{
	// struct that holds info about swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1; // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;	// output window
	swapChainDesc.SampleDesc.Count = 1;	// multi samples
	swapChainDesc.Windowed = !shouldFullscreen;	// Windowed or full screen
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// allow switching to and from full screen

	// create the device, swap chain and device context
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&s_dxgiSwapChain,
		&s_d3dDevice,
		NULL,
		&s_d3dDeviceContext);
}

bool R_SetupBackBuffer()
{
	HRESULT result = S_OK;

	// get address of the back buffer
	result = s_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&s_BackBuffer);

	if (FAILED(result))
	{
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to get back buffer from swap chain");
		return false;
	}

	// use the back buffer address as the render target
	s_d3dDevice->CreateRenderTargetView(s_BackBuffer, NULL, &s_d3dBackBufferRenderTargetView);

	return SUCCEEDED(result);
}

bool R_SetupDepthStencilBuffer(UINT screenWidth, UINT screenHeight)
{
	HRESULT hresult = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hresult = s_d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &s_depthStencilBuffer);

	if (FAILED(hresult)) {
		C_PrintLn(CON_CHANNEL_RENDER | CON_CHANNEL_ERROR, "Unable to create depth stencil buffer");
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hresult = s_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &s_depthStencilState);

	if (FAILED(hresult)) {
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create depth stencil state");
		return false;
	}

	s_d3dDeviceContext->OMSetDepthStencilState(s_depthStencilState, 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hresult = s_d3dDevice->CreateDepthStencilView(s_depthStencilBuffer, &depthStencilViewDesc, &s_depthStencilView);

	if (FAILED(hresult)) {
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create depth stencil view");
		return false;
	}

	s_d3dDeviceContext->OMSetRenderTargets(1, &s_d3dBackBufferRenderTargetView, s_depthStencilView);

	return SUCCEEDED(hresult);
}

bool R_SetupRasterizer( bool wireFrameEnabled )
{
	HRESULT result = S_OK;

	// setup how the polygons will be drawn
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = wireFrameEnabled ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	s_d3dRasterizerState = 0;
	result = s_d3dDevice->CreateRasterizerState(&rasterizerDesc, &s_d3dRasterizerState);

	if (FAILED(result) || s_d3dRasterizerState == 0)
	{
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create rasterize state");
		return false;
	}

	s_d3dDeviceContext->RSSetState(s_d3dRasterizerState);

	return SUCCEEDED(result);
}

void R_SetupViewPort(UINT screenWidth, UINT screenHeight)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (FLOAT)screenWidth;
	viewPort.Height = (FLOAT)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	s_d3dDeviceContext->RSSetViewports(1, &viewPort);
}

bool R_SetupBlending()
{
	HRESULT result = S_OK;

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = s_d3dDevice->CreateBlendState(&blendStateDescription, &s_alphaEnableBlendingState);

	if (FAILED(result)) {
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create blend state");
		return false;
	}

	return SUCCEEDED(result);
}

void R_Init(HWND hWnd, UINT screenWidth, UINT screenHeight)
{
	R_SetupSwapChain(hWnd, screenWidth, screenHeight, false);
	R_SetupBackBuffer();
	R_SetupDepthStencilBuffer(screenWidth, screenHeight);
	R_SetupRasterizer(false);
	R_SetupViewPort(screenWidth, screenHeight);
	R_SetupBlending();

	C_PrintLn(CON_CHANNEL_RENDER, "R_Init() complete");
}

void R_Frame()
{
	// clear the back buffer
	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	s_d3dDeviceContext->ClearRenderTargetView(s_d3dBackBufferRenderTargetView, clearColor);

	// clear the depth and stencil buffer
	s_d3dDeviceContext->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// switch the back buffer and the front buffer
	s_dxgiSwapChain->Present(1, 0);

	C_PrintLn(CON_CHANNEL_RENDER,"R_Frame() complete");
}

void R_Shutdown()
{
	C_PrintLn(CON_CHANNEL_RENDER,"R_Shutdown() complete");
}