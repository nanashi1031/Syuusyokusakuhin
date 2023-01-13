#pragma once

#include "Stage.h"
#include "Collision.h"

// ステージメイン
class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain() override;

    void Update(float elapsedTime) override;
    void Render(RenderContext rc, ModelShader* shader) override;

private:
};