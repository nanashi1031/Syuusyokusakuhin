#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

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

	// �������v�Z����
	template<typename T>
	static int CountDigits(T number)
	{
		int digit = 0;
		int num = static_cast<int>(number);

		while (num > 0)
		{
			num /= 10;
			digit++;
		}

		return digit;
	};

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

	// ���[���h���W����X�N���[�����W�ւ̕ϊ�
	static DirectX::XMFLOAT2 ConvertWorldToScreen(DirectX::XMFLOAT3 worldPosition,
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);
};