#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Character.h"

// プレイヤー
class Player : public Character
{
private:
    enum class Animation
    {

    };

public:
    Player();
    ~Player() override;

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void DrawDebugPrimitive();

    //デバッグ用GUI描画
    void DrawDebugGUI();


private:
    // 移動入力処理
    void InputMove(float elapsedTime);

    // ゲッター　セッター
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    Model* model = nullptr;

    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);
};