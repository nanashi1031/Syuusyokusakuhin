#pragma once
#include <DirectXMath.h>

// Float�̌v�Z
class Mathf
{
public:
	// ���`���
	static float LerpFloat(float start, float end, float time);
	static DirectX::XMFLOAT3 LerpFloat3(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float time);

	// ���ʕ��
	static DirectX::XMFLOAT3* SphereLinear(DirectX::XMFLOAT3* out, DirectX::XMFLOAT3* start, DirectX::XMFLOAT3* end, float t);

	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);

	// FLOAT3�̘a
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̍� A-B
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3��float�̏�
	static DirectX::XMFLOAT3 MultiplyFloat3Float(DirectX::XMFLOAT3 float3A, float floatB);

	// FLOAT3�̏�
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3�̏�
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3����float�̏�
	static DirectX::XMFLOAT3 DivideFloat3Float(DirectX::XMFLOAT3 float3A, float floatB);

	// FLOAT3�̓��
	static DirectX::XMFLOAT3 SqFloat3(DirectX::XMFLOAT3 float3A);

	// �Ώ�B����Ώ�A�܂ł̋������v�Z����
	static DirectX::XMFLOAT3 CalculateLength(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// ���K��
	static DirectX::XMFLOAT3 CalculateNormalize(DirectX::XMFLOAT3 float3A);

	// �}�C�i�X�Ȃ�v���X�ɂ���
	static DirectX::XMFLOAT3 MakePlusFloat3(DirectX::XMFLOAT3 float3A);

	// ���������߂� floatA �� floatB
	static float Percentage(float floatA, float floatB);

	// �v���C���[�̍U���̃_���[�W�v�Z
	static float PlayerAttackDamageCalculation(float attackPower, float defensePower);

	// �v���C���[�ւ̃_���[�W�v�Z
	static float PlayerDamageCalculation(float attackPower, float defensePower);
};