#include <d3d11.h>

#include "console.h"
#include "r_model.h"
#include "r_camera.h"

void R_LoadIndexBuffer(ID3D11Device *d3dDevice, XModel *model)
{
	ID3D11Buffer *indexBuffer;

	// description
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * model->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// buffer
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = model->indices;
	HRESULT hr = d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(hr))
	{
		C_PrintLn(CON_CHANNEL_ERROR | CON_CHANNEL_RENDER, "Unable to create index buffer" );
	}

	model->renderingResources->indexBuffer = indexBuffer;
}

void R_LoadVertexBuffer(ID3D11Device *d3dDevice, XModel *model)
{
	ID3D11Buffer *vertexBuffer;

	// desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
	vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;								// write access access by CPU and GPU
	vertexBuffDesc.ByteWidth = sizeof(XVertex) * model->vertexCount;		// size is the VERTEX struct
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// use as a vertex buffer
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// allow CPU to write in buffer

	// buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = model->vertices;
	d3dDevice->CreateBuffer(&vertexBuffDesc, &vertexData, &vertexBuffer);

	model->renderingResources->vertexBuffer = vertexBuffer;
}

void R_InitConstantBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, XModel *model)
{
	ID3D11Buffer *constantBuffer;
	XModelRenderingResources *renderingResources = model->renderingResources;

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferMatrices);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	HRESULT hr = d3dDevice->CreateBuffer(&constantBufferDesc, NULL, &constantBuffer);
	if (FAILED(hr)) {
		C_PrintLn(CON_CHANNEL_RENDER | CON_CHANNEL_ERROR, "Unable to create constant buffer");
	}
	renderingResources->constantBuffer = constantBuffer;
}

void R_UpdateConstantBuffer(ID3D11DeviceContext *d3dDeviceContext, XModel *model)
{
	XModelRenderingResources *renderingResources = model->renderingResources;

	XMMATRIX world = DirectX::XMMatrixTranslationFromVector(model->position);
	XMMATRIX scale = DirectX::XMMatrixScalingFromVector(model->scale);
	XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(model->angles);

	XMMATRIX view = R_GetViewMatrix();
	XMMATRIX projection = R_GetProjectionMatrix();

	ConstantBufferMatrices constantBuff;
	constantBuff.scale = DirectX::XMMatrixTranspose(scale);
	constantBuff.rotation = DirectX::XMMatrixTranspose(rotation);
	constantBuff.world = DirectX::XMMatrixTranspose(world);
	constantBuff.view = DirectX::XMMatrixTranspose(view);
	constantBuff.projection = DirectX::XMMatrixTranspose(projection);
	d3dDeviceContext->UpdateSubresource(renderingResources->constantBuffer, 0, NULL, &constantBuff, 0, 0);

	d3dDeviceContext->VSSetConstantBuffers(0, 1, &renderingResources->constantBuffer);
}

void R_InitializeModel(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, XModel *model)
{
	R_InitConstantBuffer(d3dDevice, d3dDeviceContext, model);
	R_LoadVertexBuffer(d3dDevice, model);
	R_LoadIndexBuffer(d3dDevice, model);
}

void R_RenderModel(ID3D11DeviceContext *d3dDeviceContext, XModel *model)
{
	R_UpdateConstantBuffer(d3dDeviceContext, model);
	XModelRenderingResources *modelRenderingResources = model->renderingResources;

	// input layout
	d3dDeviceContext->IASetInputLayout(modelRenderingResources->inputLayout);

	// select which vertex buffer to display
	UINT stride = sizeof(XVertex);
	UINT offset = 0;

	// set to use vertex bufer and index buffer
	d3dDeviceContext->IASetVertexBuffers(0, 1, &modelRenderingResources->vertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(modelRenderingResources->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// select which primtive type we are using
	d3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// shaders
	d3dDeviceContext->IASetInputLayout(modelRenderingResources->vertexShader->layout);
	d3dDeviceContext->VSSetShader(modelRenderingResources->vertexShader->shader, 0, 0);
	d3dDeviceContext->PSSetShader(modelRenderingResources->pixelShader->shader, 0, 0);

	// draw the vertex buffer to the back buffer using the index buffer
	d3dDeviceContext->DrawIndexed(model->indexCount, 0, 0);
}