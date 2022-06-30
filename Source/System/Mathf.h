#pragma once
#include <DirectXMath.h>

// Floatの計算
class Mathf
{
public:
	// 指定範囲のランダム値を計算する
	static float RandomRange(float min, float max);

	// FLOAT3の和
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の差
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の乗
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の商
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の二乗
	static DirectX::XMFLOAT3 SqFloat3(DirectX::XMFLOAT3 float3A);
};