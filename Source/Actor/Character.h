#pragma once
#include "Object.h"

// キャラクター
class Character : public Object
{
public:
    Character() {}
    ~Character() override {};

    void Update(float elapsedTime);
};