#pragma once
#include <DirectXMath.h>

// FloatΜvZ
class Mathf
{
public:
	float Lerp(float a, float b, float t);

	// wθΝΝΜ_lπvZ·ι
	static float RandomRangeFloat(float min, float max);

	// FLOAT3Μa
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3Μ·
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3Μζ
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3Μ€
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3Μρζ
	static DirectX::XMFLOAT3 SqFloat3(DirectX::XMFLOAT3 float3A);
};