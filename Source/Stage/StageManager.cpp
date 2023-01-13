#include "StageManager.h"

void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

void StageManager::Render(RenderContext rc, ModelShader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(rc, shader);
    }
}


void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }
    stages.clear();
}