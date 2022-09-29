#pragma once
#include "Player.h"

class Insect : public Character
{
    Insect();
    ~Insect();

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    // プレイヤーの剣に追尾
    void PlayerWeaponTracking(float elapsedTime);

    // ゲッター セッター

private:
    std::unique_ptr<Model> model = nullptr;

    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);
};