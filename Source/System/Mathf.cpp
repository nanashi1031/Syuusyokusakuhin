#include <stdlib.h>
#include "Mathf.h"
#include <time.h>
#include "Extract.h"

float Mathf::LerpFloat(float start, float end, float time)
{
	return start * (1.0f - time) + (end * time);
}

DirectX::XMFLOAT3 Mathf::LerpFloat3(
	DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float time)
{
	DirectX::XMFLOAT3 outLerp;
	outLerp.x = start.x * (1.0f - time) + (end.x * time);
	outLerp.y = start.y * (1.0f - time) + (end.y * time);
	outLerp.z = start.z * (1.0f - time) + (end.z * time);

	return outLerp;
}

// 球面補間
DirectX::XMFLOAT3* Mathf::SphereLinear(
	DirectX::XMFLOAT3* out, DirectX::XMFLOAT3* start, DirectX::XMFLOAT3* end, float t)
{
	DirectX::XMVECTOR vectorS, vectorE;

	vectorS = DirectX::XMLoadFloat3(start);
	vectorE = DirectX::XMLoadFloat3(end);
	vectorS = DirectX::XMVector3Normalize(vectorS);
	vectorE = DirectX::XMVector3Normalize(vectorE);


	// 2ベクトル間の角度（鋭角側）
	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(vectorS, vectorE);
	float dotdot;
	DirectX::XMStoreFloat(&dotdot, dot);
	double anger = static_cast<double>(dotdot);
	double angle = acos(anger);

	// sinθ
	double SinTh = sin(angle);
	// SinThの逆数を用意 1からShinThを割るとShinThの逆数になる
	double ShinThReciprocal = 1.0f / SinTh;
	// 補間係数
	double Ps = sin(static_cast<double>(angle * (1 - t)));
	double Pe = sin(static_cast<double>(angle * t));

	DirectX::XMStoreFloat3(
		out, DirectX::XMVectorScale(DirectX::XMVectorAdd(
			DirectX::XMVectorScale(vectorS, static_cast<float>(Ps)),
				DirectX::XMVectorScale(vectorE, static_cast<float>(Pe))),
			static_cast<float>(ShinThReciprocal)));
	// ベクターをShinThの逆数でかけることで(ベクター ÷ ShinTh)になる

	// 一応正規化して球面線形補間に
	DirectX::XMVECTOR outVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(out));
	DirectX::XMStoreFloat3(out, outVec);

	return out;
}

float Mathf::RandomRange(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float num = static_cast<float>(rand()) / RAND_MAX;

	return min + (max - min) * num;
}

DirectX::XMFLOAT3 Mathf::AddFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x + float3B.x;
	outFloat3.y = float3A.y + float3B.y;
	outFloat3.z = float3A.z + float3B.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::SubtractFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x - float3B.x;
	outFloat3.y = float3A.y - float3B.y;
	outFloat3.z = float3A.z - float3B.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::MultiplyFloat3Float(DirectX::XMFLOAT3 float3A, float floatB)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x * floatB;
	outFloat3.y = float3A.y * floatB;
	outFloat3.z = float3A.z * floatB;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::MultiplyFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x * float3B.x;
	outFloat3.y = float3A.y * float3B.y;
	outFloat3.z = float3A.z * float3B.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::DivideFloat3(DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x / float3B.x;
	outFloat3.y = float3A.y / float3B.y;
	outFloat3.z = float3A.z / float3B.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::DivideFloat3Float(DirectX::XMFLOAT3 float3A, float floatB)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x / floatB;
	outFloat3.y = float3A.y / floatB;
	outFloat3.z = float3A.z / floatB;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::SqFloat3(DirectX::XMFLOAT3 float3A)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x * float3A.x;
	outFloat3.y = float3A.y * float3A.y;
	outFloat3.z = float3A.z * float3A.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::CalculateLength(
	DirectX::XMFLOAT3 float3A, DirectX::XMFLOAT3 float3B)
{
	DirectX::XMFLOAT3 length = SubtractFloat3(float3A, float3B);
	float square =
		sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
	DirectX::XMFLOAT3 outObjectLength =
		DirectX::XMFLOAT3(length.x / square, length.y / square, length.z / square);

	return outObjectLength;
}

DirectX::XMFLOAT3 Mathf::CalculateNormalize(DirectX::XMFLOAT3 float3A)
{
	DirectX::XMFLOAT3 length = SqFloat3(float3A);
	float dist = sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
	DirectX::XMFLOAT3 out = DivideFloat3Float(length, dist);

	return out;
}

DirectX::XMFLOAT3 Mathf::MakePlusFloat3(DirectX::XMFLOAT3 float3A)
{
	DirectX::XMFLOAT3 outFloat3 = float3A;
	if (float3A.x < 0) outFloat3.x = -outFloat3.x;
	if (float3A.y < 0) outFloat3.y = -outFloat3.y;
	if (float3A.z < 0) outFloat3.z = -outFloat3.z;

	return outFloat3;
}

float Mathf::Percentage(float floatA, float floatB)
{
	float outfloat = floatA / floatB;
	return outfloat;
}

float Mathf::PlayerAttackDamageCalculation(float attackPower, float defensePower)
{
	if (Extract::Instance().GetExtract(ExtractColor::Red) >= 0.00f)
		attackPower *= 20.0f;
	float outResult = attackPower - defensePower;
	if (outResult < 0.0f)
		outResult = 0.0f;

	return outResult;
}

float Mathf::PlayerDamageCalculation(float attackPower, float defensePower)
{
	if (Extract::Instance().GetExtract(ExtractColor::Orange) >= 0.00f)
		defensePower *= 1.2f;
	float outResult = attackPower - defensePower;
	if (outResult < 0.0f)
		outResult = 0.0f;

	return outResult;
}

DirectX::XMFLOAT2 Mathf::ConvertWorldToScreen(DirectX::XMFLOAT3 worldPosition,
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numVieports = 1;
	dc->RSGetViewports(&numVieports, &viewport);
	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
	// ワールド座標からスクリーン座標へ変換
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,			//ワールド座標
		viewport.TopLeftX,		//ビューポート左上X位置
		viewport.TopLeftY,		//ビューポート左上Y位置
		viewport.Width,			//ビューポート幅
		viewport.Height,		//ビューポート高さ
		viewport.MinDepth,		//深度幅の範囲を表す最小値
		viewport.MaxDepth,		//深度幅の範囲を表す最大値
		Projection,				//プロジェクション行列
		View,					//ビュー行列
		World					//ワールド行列
	);

	// スクリーン座標
	DirectX::XMFLOAT2 screenPosition;
	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	return screenPosition;
}