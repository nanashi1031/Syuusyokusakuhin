#pragma once

#include "Graphics.h"

// �R���W����
class Collision
{
public:
    // ���C�L���X�g�̃q�b�g����
    struct HitResult
    {
        DirectX::XMFLOAT3 collisionPosition = { 0, 0, 0 };
        DirectX::XMFLOAT3 collisionNormal = { 0, 0, 0 };
        float collisionDistance = 0.0f;
        int collisionMaterialIndex = -1;
    };

public:
    // ���Ƌ��̓����蔻��
    static bool IntersectSphereVsSpherer(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // ���C�ƃ��f���̌�������
    static bool IntersecrRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};