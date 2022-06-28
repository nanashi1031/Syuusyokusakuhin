#pragma once
#include <DirectXMath.h>

// Float�̌v�Z
class Mathf
{
public:
	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);

	// DirectX::XMFLOAT3�̘a
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3�̍�
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3�̏�
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// DirectX::XMFLOAT3�̏�
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

};