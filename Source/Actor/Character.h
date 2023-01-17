#pragma once
#include "Object.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "StateMachine.h"

// キャラクター
class Character : public Object
{
private:
    struct CollisionNodes
    {
        char* name = nullptr;
        float radius = 0.0f;
        bool cameraTargetFlag = 0;
        float defensePower = 0.0f;
        int extractColor = 0;
    };
    std::vector<CollisionNodes> collisionNodes;

public:
    Character() {}
    virtual ~Character() override {}

    // 目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedRate);

    // 一定数回転する
    void Ratate(DirectX::XMFLOAT3 rotateValue, float time);

    // ダメージを与える
    bool ApplyDamage(const int damage, const float invincibleTime);

    // 衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // ゲッター、セッター
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHealth() const { return this->health; }
    void SetHealth(float f) { this->health = f; }

    const float GetMaxHealth() const { return this->maxHealth; }
    void SetMaxHealth(float f) { this->maxHealth = f; }

    float GetMoveFlag() const { return moveFlag; }

    float GetNotStand() const { return notStand; }

    DirectX::XMFLOAT3 GetTargetPosition() const {return this->targetPosition;}
    void SetTargetPosition(DirectX::XMFLOAT3 f3) { this->targetPosition = f3; }

    float GetMoveSpeed() const { return this->moveSpeed; }
    void SetMoveSpeed(float f) { this->moveSpeed = f; }

    float GetMoveVecX() const { return this->moveVecX; }
    void SetMoveVecX(float f) { this->moveVecX = f; }

    float GetMoveVecZ() const { return this->moveVecZ; }
    void SetMoveVecZ(float f) { this->moveVecZ = f; }

    void SetVerocity(DirectX::XMFLOAT3 f3) { this->velocity = f3; }
    void SetClearVerocity() { this->velocity.x = 0, this->velocity.y = 0, this->velocity.z = 0; }

    DirectX::XMFLOAT3 GetBeforePosition() const { return this->beforPosition; }

    std::vector<CollisionNodes> GetCollisionNodes() const { return this->collisionNodes; }
    void SetCollisionNodes(char* name, float radius, bool cameraTargetFlag = false)
    {
        CollisionNodes collisionNode;
        collisionNode.name = name;
        collisionNode.radius = radius;
        collisionNode.cameraTargetFlag = cameraTargetFlag;
        collisionNodes.emplace_back(collisionNode);
    }
    int GetCollisionNodesCameraTargetFlagTotal()
    {
        int total = 0;
        for (int i = 0; i < this->collisionNodes.size(); i++)
        {
            if (this->collisionNodes[i].cameraTargetFlag)
            {
                total++;
            }
        }
        return total;
    }

    //ステートマシン取得
    StateMachine* GetStateMachine() const { return stateMachine; }

    // モデル取得
    Model* GetModel() const { return model; }

    // ノード情報取得
    Model::Node* GetNode(const char* nodeName) const
    {
        Model::Node* node = model->FindNode(nodeName);
        return node;
    }

    // ノードの位置取得
    DirectX::XMFLOAT3 GetNodePosition(const Model::Node* node) const
    {
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43);
        return nodePosition;
    }

protected:
    void Move(float vx, float vz, float speed);
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // 着地した時に呼ばれる
    virtual void OnLanding() {}

    // ダメージを受けたときに呼ばれる
    virtual void OnDamaged() {}

    // 死亡したときに呼ばれる
    virtual void OnDead() {};

    //無敵時間
    void UpdateInvincibleTime(float elapsedTime);

    // 速力処理更新
    void UpdateVelocity(float elapsedTime);

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

    //無敵時間
    float invincibleTimer = 0.0f;
    bool invincible = false;

    // 吹き飛ばす力がこの数字以上なら吹き飛ぶ
    float notStand = 0.0f;

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

    DirectX::XMFLOAT3 beforPosition = { 0, 0, 0 };

    float moveFlag = 0.0f;

    BehaviorTree* aiTree = nullptr;
    BehaviorData* behaviorData = nullptr;
    NodeBase* activeNode = nullptr;
    StateMachine* stateMachine = nullptr;
};