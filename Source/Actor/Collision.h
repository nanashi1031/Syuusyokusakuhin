#pragma once

#include "Graphics.h"

// レイキャストのヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 collisionPosition = { 0, 0, 0 };
    DirectX::XMFLOAT3 collisionNormal = { 0, 0, 0 };
    float collisionDistance = 0.0f;
    int collisionMaterialIndex = -1;
};

// コリジョン
class Collision
{
public:
    // 球と球の当たり判定
    static bool IntersectSphereVsSpherer(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // レイとモデルの交差判定
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};