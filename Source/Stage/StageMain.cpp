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

void StageMain::Render(RenderContext rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}