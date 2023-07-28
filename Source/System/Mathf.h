#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

// Floatの計算
class Mathf
{
public:
	// 線形補間
	static float LerpFloat(float start, float end, float time);
	static DirectX::XMFLOAT3 LerpFloat3(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float time);

	// 球面補間
	static DirectX::XMFLOAT3* SphereLinear(DirectX::XMFLOAT3* out, DirectX::XMFLOAT3* start, DirectX::XMFLOAT3* end, float t);

	// 指定範囲のランダム値を計算する
	static float RandomRange(float min, float max);

	// 桁数を計算する
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

	// FLOAT3の和
	static DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の差 A-B
	static DirectX::XMFLOAT3 SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3とfloatの乗
	static DirectX::XMFLOAT3 MultiplyFloat3Float(DirectX::XMFLOAT3 float3A, float floatB);

	// FLOAT3の乗
	static DirectX::XMFLOAT3 MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3の商
	static DirectX::XMFLOAT3 DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// FLOAT3からfloatの商
	static DirectX::XMFLOAT3 DivideFloat3Float(DirectX::XMFLOAT3 float3A, float floatB);

	// FLOAT3の二乗
	static DirectX::XMFLOAT3 SqFloat3(DirectX::XMFLOAT3 float3A);

	// 対象Bから対象Aまでの距離を計算する
	static DirectX::XMFLOAT3 CalculateLength(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B);

	// 正規化
	static DirectX::XMFLOAT3 CalculateNormalize(DirectX::XMFLOAT3 float3A);

	// マイナスならプラスにする
	static DirectX::XMFLOAT3 MakePlusFloat3(DirectX::XMFLOAT3 float3A);

	// 割合を求める floatA ÷ floatB
	static float Percentage(float floatA, float floatB);

	// プレイヤーの攻撃のダメージ計算
	static float PlayerAttackDamageCalculation(float attackPower, float defensePower);

	// プレイヤーへのダメージ計算
	static float PlayerDamageCalculation(float attackPower, float defensePower);

	// ワールド座標からスクリーン座標への変換
	static DirectX::XMFLOAT2 ConvertWorldToScreen(DirectX::XMFLOAT3 worldPosition,
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);
};