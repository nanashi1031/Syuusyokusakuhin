#pragma once
#include "Object.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

// キャラクター
class Character : public Object
{
public:
    Character() {}
    virtual ~Character() override {}

    // ゲッター、セッター
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHealth() const { return this->health; }
    void SetHealth(float f) { this->health = f; }

    const float GetMaxHealth() const { return this->maxHealth; }
    void SetMaxHealth(float f) { this->maxHealth = f; }

    // ターゲットポジション設定
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // ターゲットポジション取得
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

    // 目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedRate);

    // モデル取得
    Model* GetModel() { return model; }

    // ノード情報取得
    Model::Node* GetNode(const char* nodeName) const
    {
        Model::Node* node = model->FindNode(nodeName);
        return node;
    }

    // アニメーション再生テンプレート
    template<typename T>
    void SetPlayAnimation(T animSuu, bool loop, float blendSeconds = 0.2f)
    {
        model->PlayAnimation(static_cast<int>(animSuu), loop, blendSeconds);
    }

protected:
    void Move(float vx, float vz, float speed);
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // 死亡した時に呼ばれる
    virtual void OnDead() {}

    // 速力処理更新
    void UpdateVelocity(float elapsedTime);

    // 着地した時に呼ばれる
    virtual void OnLanding() {}

private:
    // 垂直速力処理更新
    void UpdateVerticalVelocity(float elapsedFrame);
    // 垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);
    // 水平速力処理更新
    void UpdateHorizontalVelocity(float elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };

    float radius = 0.0f;
    float height = 0.0f;

    // ステータス
    float health = 0.0f;
    float maxHealth = 0.0f;

    float	stepOffset = 1.0f;
    float	gravity = -1.0f;
    bool	isGround = false;
    float	slopeRate = 1.0f;
    float	friction = 0.5f;
    float	acceleration = 1.0f;
    float	maxMoveSpeed = 5.0f;
    float	moveVecX = 0.0f;
    float	moveVecZ = 0.0f;
    float	airControl = 0.3f;

    float moveSpeed = 0.3f;
    float turnSpeed = DirectX::XMConvertToRadians(360);

    BehaviorTree* aiTree = nullptr;
    BehaviorData* behaviorData = nullptr;
    NodeBase* activeNode = nullptr;
};