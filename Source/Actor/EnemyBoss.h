#pragma once

#include "Enemy.h"
#include "Model.h"

class EnemyBoss : public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss() override;

    void Update(float elapsedTime) override;
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    void DrawDebugPrimitive() override;

private:
    Model* model = nullptr;
};