#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Character.h"
#include "PlayerManager.h"

class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy()override {}

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive() {};

    virtual void DrawDebugGUI() {};

    //�j��
    void Destroy();

    // �Q�b�^�[�@�Z�b�^�[
    // �v���C���[�̈ʒu�擾
    DirectX::XMFLOAT3 GetPlayerPosition()
    {
        return PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
    }

    // ���s�^�C�}�[�ݒ�
    void SetRunTimer(float timer) { runTimer = timer; }

    // ���s�^�C�}�[�擾
    float GetRunTimer() { return runTimer; }

    // �v���C���[���G
    bool SearchPlayer();

    // �^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    // �U���͈͎擾
    float GetAttackRange() { return attackRange; }

private:
    DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
    float territoryRange = 10.0f;
    float searchRange = 5.0f;
    float attackRange = 1.5f;
    float runTimer = 0.0f;
};