#include <d3d11.h>

#include "r_camera.h"

using namespace DirectX;

static XMVECTOR s_eye;
static XMVECTOR s_lookat;
static XMVECTOR s_up;

static XMMATRIX s_projectionMatrix;

void R_InitCamera(UINT screenWidth, UINT screenHeight)
{
	s_eye = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
	s_lookat = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	s_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	s_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(XM_PI  * 0.4, (FLOAT)screenWidth / (FLOAT)screenHeight, 0.1f, 1000.0f);
}

XMMATRIX R_GetProjectionMatrix()
{
	return s_projectionMatrix;
}

XMMATRIX R_GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(s_eye, s_lookat, s_up);
}

XMVECTOR R_GetNormalizedViewDirection()
{
	return XMVector3Normalize(XMVectorSubtract(s_lookat, s_eye));
}

void R_MoveCamera(XMVECTOR moveDelta)
{
	s_eye = XMVectorAdd(s_eye, moveDelta);
	s_lookat = XMVectorAdd(s_lookat, moveDelta);
}

void R_MoveCameraInViewDirection(float moveUnits)
{
	R_MoveCamera(R_GetNormalizedViewDirection() * moveUnits);
}

void R_MoveCameraInUpDirection(float moveUnits)
{
	R_MoveCamera(XMVector3Normalize(s_up) * moveUnits);
}

void R_MoveCameraInRightDirection(float moveUnits)
{
	XMVECTOR rightDirection = XMVector3Cross(R_GetNormalizedViewDirection(), s_up) * -1;
	R_MoveCamera(rightDirection * moveUnits);
}