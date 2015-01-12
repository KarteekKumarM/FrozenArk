#include <d3d11.h>
#include "r_shader.h"
#include <DirectXMath.h>
using namespace DirectX;

struct XVertex
{
	XMFLOAT3 position;
};

struct XModelRenderingResources
{
	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *constantBuffer;
	XVertexShader *vertexShader;
	XPixelShader *pixelShader;
};

struct XModel
{
	XMVECTOR position;
	XMVECTOR angles;	// roll, pitch, yaw
	XMVECTOR scale;

	unsigned int vertexCount;
	XVertex *vertices;

	unsigned int indexCount;
	WORD *indices;

	wchar_t *vertexShaderPath;
	wchar_t *pixelShaderPath;

	XModelRenderingResources *renderingResources;
};

void R_InitializeModel( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, XModel *model );
void R_RenderModel( ID3D11DeviceContext *d3dDeviceContext, XModel *model );