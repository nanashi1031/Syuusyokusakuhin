#pragma once
#include "Object.h"

// キャラクター
class Character : public Object
{
public:
    Character() {}
    ~Character() override {};

    // ゲッター、セッター
    const float GetRadius() const { return this->radius; }

    const float GetHeight() const { return this->height; }

    const float GetHp() const { return this->hp; }
    void SetHp(float f) { this->hp = f; }

protected:
    float radius = 0.0f;
    float height = 0.0f;

    // ステータス
    float hp = 0.0f;
};