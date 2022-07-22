#include <stdlib.h>
#include "Mathf.h"

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

float Mathf::RandomRange(float min, float max)
{
	// 0.0Å`1.0ÇÃä‘Ç‹Ç≈ÇÃÉâÉìÉ_ÉÄíl
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

template <typename T>
static T Mathf::BubbleSort(T& all, int size)
{

}