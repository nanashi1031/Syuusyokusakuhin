#pragma once

#include "Graphics.h"
#include "Character.h"

// ���C�L���X�g�̃q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 normal = { 0, 0, 0 };
    float distance = 0.0f;
    int materialIndex = -1;
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

    // �~��A->�~��B�̓����蔻��
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // ���ƃm�[�hB�̓����蔻��
    static bool IntersectSphereVsNode(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const Character* characterB,
        const char* nodeNameB,
        const float nodeRadiusB,
        DirectX::XMFLOAT3& outPosition);

    // �~���ƃm�[�hB(�~��)�̓����蔻��
    static bool IntersectCylinderVsNodeCylinder(
        const DirectX::XMFLOAT3 positionA,
        const float radiusA,
        const float heightA,
        const DirectX::XMFLOAT3 positionB,
        const float nodeRadiusB,
        const float nodeHeightB,
        DirectX::XMFLOAT3& outPosition);

    // �m�[�hA�Ƌ��̓����蔻��
    static bool IntersectSphereVsNode(
        const Character* characterA,
        const char* nodeNameA,
        const float nodeRadiusA,
        const DirectX::XMFLOAT3 positionB,
        const float radiusB,
        DirectX::XMFLOAT3& outPosition);

    // �m�[�hA����m�[�hB�ւ̍U������
    static bool AttackNodeVsNode(
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