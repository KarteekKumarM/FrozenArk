#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

struct ConstantBufferMatrices
{
	XMMATRIX scale;
	XMMATRIX rotation;
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct XShader
{
	unsigned int id;
	LPCWSTR fileName;
	ID3DBlob *shaderBlob;
};

struct XVertexShader
{
	XShader common;
	ID3D11InputLayout *layout;
	ID3D11VertexShader *shader;
};

struct XPixelShader
{
	XShader common;
	ID3D11PixelShader *shader;
};

void R_VertexShaderInit( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, const LPCWSTR vertexShaderFileName, XVertexShader *xVertexShader );
void R_PixelShaderInit( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, const LPCWSTR pixelShaderFileName, XPixelShader *xPixelShader );