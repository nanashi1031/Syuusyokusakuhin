#pragma once

#include "Graphics.h"
#include "Character.h"

// CLXgÌqbgÊ
struct HitResult
{
    DirectX::XMFLOAT3 collisionPosition = { 0, 0, 0 };
    DirectX::XMFLOAT3 collisionNormal = { 0, 0, 0 };
    float collisionDistance = 0.0f;
    int collisionMaterialIndex = -1;
};

// RW
class Collision
{
public:
    // ÆÌ½è»è
    static bool IntersectSphereVsSpherer(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // ÆÌGÈ½è»è
    static bool IntersectSphereVsSphererEst(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // m[hA©çm[hBÖÌU»è
    static void IntersectNodeVsNode(
        const Character* characterA,
        const char* nodeNameA,
        const float nodeRadiusA,
        Character* characterB,
        const char* nodeNameB,
        const float nodeRadiusB,
        float damage = 0.0f,
        float power = 0.0f);

    // CÆfÌð·»è
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};