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

    // �������͏����X�V
    void UpdateVerticalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);
    // �������͏����X�V
    void UpdateHorizontalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

    // �Q�b�^�[ �Z�b�^�[

private:
    // �v���C���[�̌��ɒǔ�
    void PlayerWeaponTracking(float elapsedTime);

private:
    Model* model = nullptr;

    const float size = 1.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);

    float extractColor = 0;

    DirectX::XMFLOAT3 weaponPosition;
};