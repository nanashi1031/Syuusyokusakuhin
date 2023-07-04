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

void StageManager::DrawDebugGUI()
{
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);

    if (ImGui::TreeNode("Stage"))
    {
        for (Stage* stage : stages)
        {
            int nodeId = 0;
            ImGui::PushID(nodeId++);
            stage->DrawDebugGUI();
            ImGui::PopID();
        }
    }
}

void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }
    stages.clear();
}