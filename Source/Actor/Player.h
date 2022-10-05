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
    void DrawDebugGUI();


    // ゲッター　セッター
    // 前方向を取得
    DirectX::XMFLOAT3 GetFront() const;

    // ノード情報取得
    Model::Node* Player::GetNode(const char* nodeName) const
    {
        Model::Node* node = model->FindNode(nodeName);
        return node;
    }

private:
    void InputMove(float elapsedTime);

    void InputAttack(float elapedTime);

    void CollisionPlayerVsEnemies();

    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);
};