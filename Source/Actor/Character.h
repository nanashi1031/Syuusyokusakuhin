#pragma once
#include "Object.h"

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

protected:
    void Move(float elapsedTime, float vx, float vz, float speed);
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
};