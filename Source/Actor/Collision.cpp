#include "Collision.h"
#include "Mathf.h"

bool Collision::IntersectSphereVsSpherer(
    DirectX::XMFLOAT3 positionA,
    float rangeA,
    DirectX::XMFLOAT3 positionB,
    float rangeB,
    DirectX::XMFLOAT3& outPosition)
{
    DirectX::XMVECTOR positionAVec = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR positionBVec = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(positionBVec, positionAVec);

    return false;
}