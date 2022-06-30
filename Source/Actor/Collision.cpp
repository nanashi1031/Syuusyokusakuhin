#include "Collision.h"
#include "Mathf.h"

// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
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