#pragma once
#include "Object.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "StateMachine.h"

// �L�����N�^�[
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

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);

    // ��萔��]����
    void Ratate(DirectX::XMFLOAT3 rotateValue, float time);

    // �_���[�W��^����
    bool ApplyDamage(const int damage, const float invincibleTime);

    // �Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // �Q�b�^�[�A�Z�b�^�[
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

    //�X�e�[�g�}�V���擾
    StateMachine* GetStateMachine() const { return stateMachine; }

    // ���f���擾
    Model* GetModel() const { return model; }

    // �m�[�h���擾
    Model::Node* GetNode(const char* nodeName) const
    {
        Model::Node* node = model->FindNode(nodeName);
        return node;
    }

    // �m�[�h�̈ʒu�擾
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

    // ���n�������ɌĂ΂��
    virtual void OnLanding() {}

    // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    // ���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {};

    //���G����
    void UpdateInvincibleTime(float elapsedTime);

    // ���͏����X�V
    void UpdateVelocity(float elapsedTime);

private:
    // �������͏����X�V
    void UpdateVerticalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);
    // �������͏����X�V
    void UpdateHorizontalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };

    float radius = 0.0f;
    float height = 0.0f;

    // �X�e�[�^�X
    float health = 0.0f;
    float maxHealth = 0.0f;

    //���G����
    float invincibleTimer = 0.0f;
    bool invincible = false;

    // ������΂��͂����̐����ȏ�Ȃ琁�����
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