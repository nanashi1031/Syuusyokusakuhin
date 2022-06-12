#include "Stage.h"

Stage::Stage()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

Stage::~Stage()
{
    delete model;
}

void Stage::Update(float elapsedTime)
{

}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}