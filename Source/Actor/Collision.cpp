#include "Collision.h"
#include "Mathf.h"

// ÆÌ½è»è
bool Collision::IntersectSphereVsSpherer(
    const DirectX::XMFLOAT3 positionA,
    const float radiusA,
    const DirectX::XMFLOAT3 positionB,
    const float radiusB,
    DirectX::XMFLOAT3& outPosition)
{
    DirectX::XMVECTOR positionAVec = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR positionBVec = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(positionBVec, positionAVec);
    DirectX::XMVECTOR lengthSqVec = DirectX::XMVector3LengthSq(vec);
    float lengthSqf;
    DirectX::XMStoreFloat(&lengthSqf, lengthSqVec);

    float range = radiusB + radiusA;
    if (lengthSqf > range * range)
    {
        return false;
    }

    vec = DirectX::XMVector3Normalize(vec);
    vec = DirectX::XMVectorScale(vec, range);
    positionBVec = DirectX::XMVectorAdd(positionAVec, vec);
    DirectX::XMStoreFloat3(&outPosition, positionBVec);

    return true;
}

// ÆÌGÈ½è»è
bool Collision::IntersectSphereVsSphererEst(
	const DirectX::XMFLOAT3 positionA,
	const float radiusA,
	const DirectX::XMFLOAT3 positionB,
	const float radiusB,
	DirectX::XMFLOAT3& outPosition)
{
	DirectX::XMVECTOR positionAVec = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR positionBVec = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(positionBVec, positionAVec);
	DirectX::XMVECTOR lengthSqVec = DirectX::XMVector3LengthSq(vec);
	float lengthSqf;
	DirectX::XMStoreFloat(&lengthSqf, lengthSqVec);

	float range = radiusB + radiusA;
	if (lengthSqf > range * range)
	{
		return false;
	}

	vec = DirectX::XMVector3NormalizeEst(vec);
	vec = DirectX::XMVectorScale(vec, range);
	positionBVec = DirectX::XMVectorAdd(positionAVec, vec);
	DirectX::XMStoreFloat3(&outPosition, positionBVec);

	return true;
}

// m[hA©çm[hBÖÌU»è
void Collision::IntersectNodeVsNode(
	const Character* characterA,
	const char* nodeNameA,
	const float nodeRadiusA,
	Character* characterB,
	const char* nodeNameB,
	const float nodeRadiusB,
	float damage,
	float power)
{
	Model::Node* nodeA = characterA->GetNode(nodeNameA);
	if (nodeA != nullptr)
	{
		DirectX::XMFLOAT3 nodePositionA = characterA->GetNodePosition(nodeA);

		Graphics::Instance().GetDebugRenderer()
			->DrawSphere(
				nodePositionA, nodeRadiusA, DirectX::XMFLOAT4(1, 0, 0, 1)
			);

		Model::Node* nodeB = characterB->GetNode(nodeNameB);
		if (nodeB != nullptr)
		{
			DirectX::XMFLOAT3 nodePositionB = characterA->GetNodePosition(nodeB);

			Graphics::Instance().GetDebugRenderer()
				->DrawSphere(
					nodePositionB, nodeRadiusB, DirectX::XMFLOAT4(0, 0, 1, 1)
				);

			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsSpherer(
				nodePositionA, nodeRadiusA,
				nodePositionB, nodeRadiusB,
				outPosition))
			{
				// _[Wð^¦é
				if (characterB->ApplyDamage(damage, 1.0f))
				{
					//@powerª¥ñ£éÍæè­¢Èç
					if (power > characterB->GetNotStand())
					{
						// characterBðÁòÎ·xNgðZo
						DirectX::XMFLOAT3 vec;
						vec.x = outPosition.x
							- nodePositionA.x;
						vec.z = outPosition.z
							- nodePositionA.z;
						float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
						vec.x /= length;
						vec.z /= length;
						// XZ½ÊÉÁòÎ·Íð©¯é
						vec.x *= power;
						vec.z *= power;
						// YûüÉàÍð©¯é
						vec.y = 5.0f;
						// ÁòÎ·
						characterB->AddImpulse(vec);
					}
				}
			}
		}
	}

	return;
}

// CÆfÌð·»è
bool Collision::IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result)
{
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);
	// [hóÔÌCÌ·³
	DirectX::XMStoreFloat(&result.collisionDistance, WorldRayLength);
	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// bVm[hæ¾
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
		// Cð[hóÔ©ç[JóÔÖÏ·
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
		// CÌ·³
		float neart;
		DirectX::XMStoreFloat(&neart, Length);
		// Op`iÊjÆÌð·»è
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;
		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;
				// Op`Ì¸_ðo
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
				// Op`ÌOÓxNgðZo
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
				// Op`Ì@üxNgðZo
				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);
				// àÏÌÊªvXÈçÎ ü«
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0) continue;
				// CÆ½ÊÌð_ðZo
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);
				if (t < .0f || t > neart) continue; // ð_ÜÅÌ£ª¡ÜÅÉvZµ½Åß£æè
				// å«¢ÍXLbv
				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T),
					Start);
				// ð_ªOp`Ìà¤É é©»è
				// PÂß
				DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, Position);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f) continue;
				// QÂß
				DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, Position);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f) continue;
				// RÂß
				DirectX::XMVECTOR V3
					= DirectX::XMVectorSubtract(C, Position);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f) continue;
				// Åß£ðXV
				neart = t;
				// ð_Æ@üðXV
				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			// [JóÔ©ç[hóÔÖÏ·
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
				WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);
			// qbgîñÛ¶
			if (result.collisionDistance > distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal,
					WorldTransform);
				result.collisionDistance = distance;
				result.collisionMaterialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.collisionPosition, WorldPosition);
				DirectX::XMStoreFloat3(&result.collisionNormal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

    return hit;
}