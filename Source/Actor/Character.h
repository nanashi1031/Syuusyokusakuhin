#pragma once
#include "Object.h"

// �L�����N�^�[
class Character : public Object
{
public:
    Character() {}
    virtual ~Character() override {};

    // �Q�b�^�[�A�Z�b�^�[
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHealth() const { return this->health; }
    void SetHealth(float f) { this->health = f; }

protected:
    float radius = 0.0f;
    float height = 0.0f;

    // �X�e�[�^�X
    float health = 0.0f;
};