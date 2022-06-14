#pragma once

#include "Stage.h"

// �X�e�[�W���C��
class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain() override;

    void Update(float elapsedTime) override;

    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

private:
    Model* model = nullptr;
};