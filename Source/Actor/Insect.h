#pragma once
#include "Player.h"

class Insect : public Character
{
public:
    enum class State
    {
        Idle,
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

    // ゲッター セッター

private:
    // プレイヤーの剣に追尾
    void PlayerWeaponTracking(float elapsedTime);

private:
    Model* model = nullptr;

    const float size = 1.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);

    DirectX::XMFLOAT3 weaponPosition;
};