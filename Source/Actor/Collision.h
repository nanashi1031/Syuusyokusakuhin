#pragma once

#include "Graphics.h"

// ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
    // ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
    static bool IntersectSphereVsSpherer(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);
};