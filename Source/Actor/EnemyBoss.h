#pragma once

#include "Enemy.h"
#include "Model.h"

class EnemyBoss : public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss() override;

    void Update(float elapsedTime) override;
    void Render(RenderContext rc, ModelShader* shader) override;

    void DrawDebugPrimitive() override;

private:
    Model* model = nullptr;
};