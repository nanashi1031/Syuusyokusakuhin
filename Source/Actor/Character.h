#pragma once
#include "Object.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "StateMachine.h"

// �L�����N�^�[
class Character : public Object
{
public:
    Character() {}
    virtual ~Character() override {}

    // �Q�b�^�[�A�Z�b�^�[
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHealth() const { return this->health; }
    void SetHealth(float f) { this->health = f; }

    const float GetMaxHealth() const { return this->maxHealth; }
    void SetMaxHealth(float f) { this->maxHealth = f; }

    float GetMoveFlag() const { return moveFlag; }

    //�X�e�[�g�}�V���擾
    StateMachine* GetStateMachine() { return stateMachine; }

    // �^�[�Q�b�g�|�W�V�����ݒ�
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // �^�[�Q�b�g�|�W�V�����擾
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);

    // ���f���擾
    Model* GetModel() { return model; }

    // �m�[�h���擾
    Model::Node* GetNode(const char* nodeName) const
    {
        Model::Node* node = model->FindNode(nodeName);
        return node;
    }

protected:
    void Move(float vx, float vz, float speed);
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // ���S�������ɌĂ΂��
    virtual void OnDead() {}

    // ���͏����X�V
    void UpdateVelocity(float elapsedTime);

    // ���n�������ɌĂ΂��
    virtual void OnLanding() {}

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

    float moveFlag = 0.0f;

    BehaviorTree* aiTree = nullptr;
    BehaviorData* behaviorData = nullptr;
    NodeBase* activeNode = nullptr;
    StateMachine* stateMachine = nullptr;
};