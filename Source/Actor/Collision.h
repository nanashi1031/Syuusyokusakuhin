#pragma once

#include "Graphics.h"
#include "Character.h"

// ���C�L���X�g�̃q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 collisionPosition = { 0, 0, 0 };
    DirectX::XMFLOAT3 collisionNormal = { 0, 0, 0 };
    float collisionDistance = 0.0f;
    int collisionMaterialIndex = -1;
};

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

    // ���Ƌ��̎G�ȓ����蔻��
    static bool IntersectSphereVsSphererEst(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // �m�[�hA����m�[�hB�ւ̍U������
    static void IntersectNodeVsNode(
        const Character* characterA,
        const char* nodeNameA,
        const float nodeRadiusA,
        Character* characterB,
        const char* nodeNameB,
        const float nodeRadiusB,
        float damage = 0.0f,
        float power = 0.0f);

    // ���C�ƃ��f���̌�������
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};