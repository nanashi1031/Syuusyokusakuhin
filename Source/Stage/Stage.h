#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"

// �X�e�[�W
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