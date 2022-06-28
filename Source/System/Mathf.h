#pragma once
#include <DirectXMath.h>

// FloatΜvZ
class Mathf
{
public:
	// wθΝΝΜ_lπvZ·ι
	static float RandomRange(float min, float max);

	// DirectX::XMFLOAT3Μa
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3Μ·
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3Μζ
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3Μ€
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

};