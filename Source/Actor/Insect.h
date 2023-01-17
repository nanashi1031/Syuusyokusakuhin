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

    // 垂直速力処理更新
    void UpdateVerticalVelocity(float elapsedFrame);
    // 垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);
    // 水平速力処理更新
    void UpdateHorizontalVelocity(float elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

    // ゲッター セッター

private:
    // プレイヤーの剣に追尾
    void PlayerWeaponTracking(float elapsedTime);

private:
    Model* model = nullptr;

    const float size = 1.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);

    float extractColor = 0;

    DirectX::XMFLOAT3 weaponPosition;
};