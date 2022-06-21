#pragma once

#include "Enemy.h"
#include "Model.h"

class EnemyBoss : Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss() override;

private:
    Model* model = nullptr;
};