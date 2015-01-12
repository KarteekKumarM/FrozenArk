#include "r_shader.h"
#include "console.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// TODO: Need to make this readable from config 
void R_CreateInputLayoutObjectForVertexBuffer( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, XVertexShader *xVertexShader )
{
	ID3D11InputLayout *layout;

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT hr = d3dDevice->CreateInputLayout( ied, sizeof( ied ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), xVertexShader->common.shaderBlob->GetBufferPointer(), xVertexShader->common.shaderBlob->GetBufferSize(), &layout );
	if ( FAILED( hr ) )
	{
		Con_PrintLn( CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Failed to create the input layout object" );
		return;
	}

	xVertexShader->layout = layout;
}

void R_VertexShaderInit( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, const LPCWSTR vertexShaderFileName, XVertexShader *xVertexShader )
{
	HRESULT hr = S_OK;
	ID3DBlob *vertexShaderBlob;
	ID3D11VertexShader *vertexShader;

	hr = D3DReadFileToBlob( vertexShaderFileName, &vertexShaderBlob );
	if ( FAILED( hr ) )
	{
		Con_PrintLn( CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to read vertex shader" );
		return;
	}

	hr = d3dDevice->CreateVertexShader( vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &vertexShader );
	if ( FAILED( hr ) )
	{
		Con_PrintLn( CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create vertex shader" );
		return;
	}

	xVertexShader->common.fileName = vertexShaderFileName;
	xVertexShader->common.shaderBlob = vertexShaderBlob;
	xVertexShader->shader = vertexShader;

	//TODO: needs fixing
	R_CreateInputLayoutObjectForVertexBuffer( d3dDevice, d3dDeviceContext, xVertexShader );
}

void R_PixelShaderInit( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, const LPCWSTR pixelShaderFileName, XPixelShader *xPixelShader )
{
	HRESULT hr = S_OK;
	ID3DBlob *pixelShaderBlob;
	ID3D11PixelShader *pixelShader;

	hr = D3DReadFileToBlob( pixelShaderFileName, &pixelShaderBlob );
	if ( FAILED( hr ) )
	{
		Con_PrintLn( CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to read pixel shader" );
		return;
	}

	hr = d3dDevice->CreatePixelShader( pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &pixelShader );
	if ( FAILED( hr ) )
	{
		Con_PrintLn( CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create pixel shader" );
		return;
	}

	xPixelShader->common.fileName = pixelShaderFileName;
	xPixelShader->common.shaderBlob = pixelShaderBlob;
	xPixelShader->shader = pixelShader;
}