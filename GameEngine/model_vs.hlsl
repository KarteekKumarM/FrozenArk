#include "model_common.hlsli"

cbuffer MatrixBuffer
{
	matrix scaleMatrix;
	matrix rotationMatrix;
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.position = mul(input.position, scaleMatrix);
	output.position = mul(input.position, rotationMatrix);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	return output;
}