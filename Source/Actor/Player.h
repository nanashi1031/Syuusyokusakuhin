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
    //デバッグ用GUI描画
    void DrawDebugGUI();

    // ゲッターセッター
    const DirectX::XMFLOAT3& GetPosition() const { return this->position; }
    void SetPosition(DirectX::XMFLOAT3& position) { this->position = position; }

    const DirectX::XMFLOAT3& GetAngle() const { return this->angle; }
    void SetAngle(DirectX::XMFLOAT3& angle) { this->angle = angle; }

    const DirectX::XMFLOAT3& GetScale() const { return this->scale; }
    void SetScale(DirectX::XMFLOAT3& scale) { this->scale; }

private:
    // スティック入力値から移動ベクトルを取得
    //DirectX::XMFLOAT3 GetMoveVec();
    void Move();

private:
    Model* model = nullptr;

    const float k_size = 0.015f;
    const float moveSpeed = 0.1f;
};