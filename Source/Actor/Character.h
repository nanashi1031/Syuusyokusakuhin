#pragma once
#include "Object.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "StateMachine.h"
#include "Extract.h"
#include "Audio.h"
// TODO:�{���͂���ȂƂ���SceneGame�ĂԂ�
#include "SceneGame.h"

// �L�����N�^�[
class Character : public Object
{
public:
    enum class Type
    {
        Non,
        Boss,
    };

private:
    struct Part
    {
        char* name = nullptr;
        float radius = 0.0f;
        bool cameraTargetFlag = 0;
        float defensePower = 0.0f;
        int extractColor = static_cast<int>(ExtractColor::None);
    };
    std::vector<Part> parts;

public:
    Character() {}
    virtual ~Character() override {}

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);

    // ��萔��]����
    void Ratate(DirectX::XMFLOAT3 rotateValue, float time);

    // �_���[�W��^����
    bool ApplyDamage(const float damage, const float invincibleTime);

    // �Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // �Q�b�^�[�A�Z�b�^�[
    float GetRadius() const { return this->radius; }

    float GetHeight() const { return this->height; }

    float GetHealth() const { return this->health; }
    void SetHealth(float f) { this->health = f; }

    float GetMaxHealth() const { return this->maxHealth; }
    void SetMaxHealth(float f) { this->maxHealth = f; }

    char* GetPursuitName() const { return this->pursuitName; }
    void SetPursuitName(char* name) { this->pursuitName = name; }
    DirectX::XMFLOAT3 GetPursuitLength() const { return this->pursuitLength; }
    void SetPursuitLength(DirectX::XMFLOAT3 length) { this->pursuitLength = length; }

    float GetMoveFlag() const { return moveFlag; }

    float GetNotStand() const { return notStand; }

    DirectX::XMFLOAT3 GetTargetPosition() const { return this->targetPosition; }
    void SetTargetPosition(DirectX::XMFLOAT3 f3) { this->targetPosition = f3; }

    float GetMoveSpeed() const { return this->moveSpeed; }
    void SetMoveSpeed(float f) { this->moveSpeed = f; }

    float GetMoveVecX() const { return this->moveVecX; }
    void SetMoveVecX(float f) { this->moveVecX = f; }
    float GetMoveVecY() const { return this->moveVecY; }
    void SetMoveVecY(float f) { this->moveVecY = f; }
    float GetMoveVecZ() const { return this->moveVecZ; }
    void SetMoveVecZ(float f) { this->moveVecZ = f; }

    DirectX::XMFLOAT3 GetVerocity() { return this->velocity; }
    void SetVerocity(DirectX::XMFLOAT3 f3) { this->velocity = f3; }
    void SetClearVerocity() { this->velocity = { 0, 0, 0 }; }

    DirectX::XMFLOAT3 GetBeforePosition() const { return this->beforPosition; }

    int GetExtractColorCast() const { return this->extractColor; }
    ExtractColor GetExtractColor()
    {
        return static_cast<ExtractColor>(this->extractColor);
    }
    template<typename T>
    void SetExtractColor(T i) { extractColor = static_cast<int>(i); }

    bool GetMovingFlag() const { return this->movingFlag; }
    void SetMovingFlag(bool b) { this->movingFlag = b; }

    bool GetAvoidFlag() const { return this->avoidFlag; }
    void SetAvoidFlag(bool b) { this->avoidFlag = b; }

    int GetLightIndex() const { return this->lightIndex; }
    void SetLightIndex(int index) { this->lightIndex = index; }

    Type GetCharacterType() const { return this->CharacterType; }

    SceneGameState GetSceneGameState() const  { return this->sceneGameState; }
    void SetSceneGameState(SceneGameState state) { this->sceneGameState = state; }

    float GetRecoveryAmount() const { return this->recoveryAmount; }
    void SetRecoveryAmount(float f) { this->recoveryAmount = f; }

    float GetInvincibleTimer() const { return this->invincibleTimer; }
    void SetInvincibleTimer(float f) { this->invincibleTimer = f; }
    // �O�������擾
    DirectX::XMFLOAT3 GetFront() const;

    // �̗͂̊���
    float GetHealthPercentage()
    {
        return this->health / this->maxHealth;
    }

    std::vector<Part> GetParts() const { return this->parts; }
    void SetParts(
        char* name, float radius,
        ExtractColor extractColor = ExtractColor::None,
        float defensePower = 0.0f,
        bool cameraTargetFlag = false
    )
    {
        Part part;
        part.name = name;
        part.radius = radius;
        part.cameraTargetFlag = cameraTargetFlag;
        part.defensePower = defensePower;
        part.extractColor = static_cast<int>(extractColor);
        parts.emplace_back(part);
    }
    void SetPartsSpecify(
        int index, char* name, float radius,
        bool cameraTargetFlag, float defensePower, int extractColor)
    {
        parts[index].name = name;
        parts[index].radius = radius;
        parts[index].cameraTargetFlag = cameraTargetFlag;
        parts[index].defensePower = defensePower;
        parts[index].extractColor = extractColor;
    }
    int GetPartsCameraTargetFlagTotal()
    {
        int total = 0;
        for (int i = 0; i < this->parts.size(); i++)
        {
            if (this->parts[i].cameraTargetFlag)
            {
                total++;
            }
        }
        return total;
    }

    // ���ʌ���
    int SearchParts(char* name)
    {
        for (int i = 0; i < parts.size(); i++)
        {
            if (parts[i].name == name)
                return i;
        }
        return -1;
    }

    template<typename T>
    AudioSource* GetAudios(T index)
    {
        int i = static_cast<int>(index);
        return audios.at(i);
    }

    //�X�e�[�g�}�V���擾
    StateMachine* GetStateMachine() const { return stateMachine; }

    // ���f���擾
    Model* GetModel() const { return this->model; }

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
    void Turn(float elapsedTime, float vx, float vz, float speed);
protected:
    void Move(float vx, float vz, float speed);


    // ���n�������ɌĂ΂��
    virtual void OnLanding() {}

    // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    // ���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {};

    // �_�E�����ɌĂ΂��
    virtual void OnDown() {};

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
    // �񕜗�
    float recoveryAmount = 30.0f;

    //���G����
    float invincibleTimer = 0.0f;
    bool invincible = false;

    // ������΂��͂����̐����ȏ�Ȃ琁�����
    float notStand = 0.0f;

    float stepOffset = 1.0f;
    float gravity = -1.0f;
    bool isGround = false;
    float slopeRate = 1.0f;
    float friction = 0.5f;
    float acceleration = 1.0f;
    float maxMoveSpeed = 5.0f;
    float moveVecX = 0.0f;
    float moveVecY = 0.0f;
    float moveVecZ = 0.0f;
    float airControl = 0.3f;

    bool movingFlag = true;
    bool avoidFlag = false;

    float moveSpeed = 0.3f;
    float turnSpeed = DirectX::XMConvertToRadians(360);

    char* pursuitName = 0;
    DirectX::XMFLOAT3 pursuitLength = { 0.0f, 0.0f, 0.0f };

    int extractColor = static_cast<int>(ExtractColor::None);
    DirectX::XMFLOAT3 beforPosition = { 0, 0, 0 };

    float moveFlag = 0.0f;

    int partsIndex = 0;

    int lightIndex = -1;

    Type CharacterType = Type::Non;
    SceneGameState sceneGameState = SceneGameState::Game;

    BehaviorTree* aiTree = nullptr;
    BehaviorData* behaviorData = nullptr;
    NodeBase* activeNode = nullptr;
    StateMachine* stateMachine = nullptr;
};