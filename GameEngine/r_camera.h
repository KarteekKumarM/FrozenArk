#include <DirectXMath.h>

void R_InitCamera(UINT screenWidth, UINT screenHeight);
DirectX::XMMATRIX R_GetViewMatrix();
DirectX::XMMATRIX R_GetProjectionMatrix();

void R_MoveCameraInViewDirection(float moveUnits);
void R_MoveCameraInUpDirection(float moveUnits);
void R_MoveCameraInRightDirection(float moveUnits);