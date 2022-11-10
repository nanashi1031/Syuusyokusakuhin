#include "StageMain.h"

StageMain::StageMain()
{
    model = new Model("Data/Model/Stage/Stage.mdl");

    scale.x = scale.y = scale.z = 0.05f;
}

StageMain::~StageMain()
{
    delete model;
}

void StageMain::Update(float elapsedTime)
{
    UpdateTransform();
    model->UpdateTransform(transform);
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}