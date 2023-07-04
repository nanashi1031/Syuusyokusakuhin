#include "StageMain.h"

StageMain::StageMain()
{
    model = std::make_shared<Model>("Data/Model/Stage/Untitled.mdl");

    scale.x = scale.y = scale.z = 0.001f;
}

StageMain::~StageMain()
{

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

void StageMain::DrawDebugGUI()
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Postion", &position.x, 0.1f);
        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);

        ImGui::DragFloat3("Scale", &scale.x, 0.0005f, 0, 1000);
    }
    ImGui::TreePop();
}