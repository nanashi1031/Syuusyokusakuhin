#pragma once
#include "Player.h"

class Insect : public Character
{
public:
    Insect();
    ~Insect();

    void Initialize();
    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    // �Q�b�^�[ �Z�b�^�[

private:
    // �v���C���[�̌��ɒǔ�
    void PlayerWeaponTracking(float elapsedTime);

private:
    Model* model = nullptr;

    const float size = 1.0f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    DirectX::XMFLOAT3 weaponPosition;
};