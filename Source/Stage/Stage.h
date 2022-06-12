#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"

// ステージ
class Stage
{
public:
    Stage();
    ~Stage();

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

private:
    Model* model = nullptr;
};