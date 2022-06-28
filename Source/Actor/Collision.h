#pragma once

#include "Graphics.h"

class Collision
{
    bool IntersectSphereVsSpherer(
        DirectX::XMFLOAT3 positionA,
        float rangeA,
        DirectX::XMFLOAT3 positionB,
        float rangeB,
        DirectX::XMFLOAT3& outPosition);
};