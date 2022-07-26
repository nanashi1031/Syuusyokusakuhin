#pragma once
#include <DirectXMath.h>

// Float�̌v�Z
class Mathf
{
public:
	// ���`���
	static float LerpFloat(float a, float b, float t);
	static DirectX::XMFLOAT3 LerpFloat3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);

	// ���ʕ��

	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);

	// FLOAT3�̘a
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̍�
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̏�
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̏�
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̓��
	static DirectX::XMFLOAT3 SqFloat3(DirectX::XMFLOAT3 float3A);

	// �Ώۂ���Ώۂ܂ł̋������v�Z����
	static DirectX::XMFLOAT3 CalculateLength(DirectX::XMFLOAT3 objectA, DirectX::XMFLOAT3 objectB);
};