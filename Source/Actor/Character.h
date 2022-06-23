#pragma once
#include "Object.h"

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

protected:
    void Move(float elapsedTime, float vx, float vz, float speed);
    void Turn(float elapsedTime, float vx, float vz, float speed);

protected:
    float radius = 0.0f;
    float height = 0.0f;

    // ステータス
    float health = 0.0f;
};