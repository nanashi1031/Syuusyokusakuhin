#pragma once

#include "Graphics.h"
#include "Character.h"

// レイキャストのヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 normal = { 0, 0, 0 };
    float distance = 0.0f;
    int materialIndex = -1;
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

    // 球と球の雑な当たり判定
    static bool IntersectSphereVsSphererEst(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // 球とノードBの当たり判定
    static bool IntersectSphereVsNode(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const Character* characterB,
        const char* nodeNameB,
        const float nodeRadiusB,
        DirectX::XMFLOAT3& outPosition);

    // ノードAと球の当たり判定
    static bool IntersectSphereVsNode(
        const Character* characterA,
        const char* nodeNameA,
        const float nodeRadiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // ノードAからノードBへの攻撃判定
    static bool AttackNodeVsNode(
        const Character* characterA,
        const char* nodeNameA,
        const float nodeRadiusA,
        Character* characterB,
        const char* nodeNameB,
        const float nodeRadiusB,
        float damage = 0.0f,
        float power = 0.0f);

    // レイとモデルの交差判定
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};