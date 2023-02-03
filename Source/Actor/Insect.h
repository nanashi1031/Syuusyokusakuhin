#pragma once
#include "Player.h"

class Insect : public Character
{
public:
    enum class State
    {
        Idle,
        Pursuit,
        Flying,
        Return,
    };

public:
    Insect();
    ~Insect();

    void Initialize();
    void Update(float elapsedTime);
    void Render(RenderContext rc, ModelShader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

private:
    // �������͏����X�V
    void UpdateVerticalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);
    // �������͏����X�V
    void UpdateHorizontalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

    void UpdateLight();

    // �v���C���[�̌��ɒǔ�
    void PlayerWeaponTracking(float elapsedTime);

private:
    DirectX::XMFLOAT3 weaponPosition;

    std::unique_ptr<AudioSource> SE_Attack;
};