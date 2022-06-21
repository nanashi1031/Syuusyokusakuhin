#pragma once
#include "Object.h"

// �L�����N�^�[
class Character : public Object
{
public:
    Character() {}
    ~Character() override {};

    // �Q�b�^�[�A�Z�b�^�[
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHp() const { return this->hp; }
    void SetHp(float f) { this->hp = f; }

protected:
    float radius = 0.0f;
    float height = 0.0f;

    // �X�e�[�^�X
    float hp = 0.0f;
};