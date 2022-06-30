#pragma once

#include "Graphics.h"

// �R���W����
class Collision
{
public:
    // ���Ƌ��̓����蔻��
    static bool IntersectSphereVsSpherer(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);
};