#include "StageMain.h"

StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
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

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}