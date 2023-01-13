#include <stdlib.h>
#include "Mathf.h"
#include <time.h>

float Mathf::LerpFloat(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

DirectX::XMFLOAT3 Mathf::LerpFloat3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t)
{
	DirectX::XMFLOAT3 lerp;
	lerp.x = a.x * (1.0f - t) + (b.x * t);
	lerp.y = a.y * (1.0f - t) + (b.y * t);
	lerp.z = a.z * (1.0f - t) + (b.z * t);

	return lerp;
}

// 球面補間
DirectX::XMFLOAT3* Mathf::SphereLinear(DirectX::XMFLOAT3* out, DirectX::XMFLOAT3* start, DirectX::XMFLOAT3* end, float t)
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
	float angle = acos(anger);

	// sinθ
	float SinTh = sin(angle);
	// SinThの逆数を用意 1からShinThを割るとShinThの逆数になる
	float ShinThReciprocal = 1.0f / SinTh;
	// 補間係数
	float Ps = sin(static_cast<double>(angle * (1 - t)));
	float Pe = sin(static_cast<double>(angle * t));

	DirectX::XMStoreFloat3(out, DirectX::XMVectorScale(DirectX::XMVectorAdd(DirectX::XMVectorScale(vectorS, Ps), DirectX::XMVectorScale(vectorE, Pe)), ShinThReciprocal));// ベクターをShinThの逆数でかけることで(ベクター ÷ ShinTh)になる

	// 一応正規化して球面線形補間に
	DirectX::XMVECTOR outVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(out));
	DirectX::XMFLOAT3 outFloat3;
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

DirectX::XMFLOAT3 Mathf::SqFloat3(DirectX::XMFLOAT3 float3A)
{
	DirectX::XMFLOAT3 outFloat3;
	outFloat3.x = float3A.x * float3A.x;
	outFloat3.y = float3A.y * float3A.y;
	outFloat3.z = float3A.z * float3A.z;

	return outFloat3;
}

DirectX::XMFLOAT3 Mathf::CalculateLength(DirectX::XMFLOAT3 objectA, DirectX::XMFLOAT3 objectB)
{
	DirectX::XMFLOAT3 length =
		SubtractFloat3(objectA, objectB);
	float square = sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
	DirectX::XMFLOAT3 objectLength = DirectX::XMFLOAT3(length.x / square, length.y / square, length.z / square);

	return objectLength;
}