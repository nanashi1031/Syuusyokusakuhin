#include "Insect.h"
#include "PlayerManager.h"
#include "Mathf.h"
#include "InsectDerived.h"
#include <imgui.h>

Insect::Insect()
{
    model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    scale.x = scale.y = scale.z = 1.0f;

    radius = 0.5f;
    height = 2.0f;
    moveSpeed = 5.0f;

    stateMachine = new StateMachine();
#pragma region ステート登録
    stateMachine->RegisterState(new InsectState::IdleState(this));
    stateMachine->RegisterState(new InsectState::FlyingState(this));
    stateMachine->RegisterState(new InsectState::ReturnState(this));
#pragma endregion
    stateMachine->SetState(State::Idle);
}

Insect::~Insect()
{

}

void Insect::Initialize()
{

}

void Insect::Update(float elapsedTime)
{
    UpdateTransform();

    model->UpdateTransform(transform);

    // 速力処理更新
    UpdateVelocity(elapsedTime);

    stateMachine->Update(elapsedTime);
    //PlayerWeaponTracking(elapsedTime);
}

void Insect::Render(RenderContext rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void Insect::DrawDebugPrimitive()
{

}

void Insect::DrawDebugGUI()
{
    ImVec2 windowPosition = { 10, 10 };
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
    ImVec2 windowSize = { 300, 300 };
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);
    //ImGui::SetNextTreeNodeOpen();

    if (ImGui::Begin("Insect", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Postion", &position.x, 0.1f);
            ImGui::DragFloat("moveSpeed", &moveSpeed, 0.1f);
            ImGui::DragFloat3("SwordPostion", &weaponPosition.x, 0.1f);
        }
    }

    ImGui::End();
}

void Insect::PlayerWeaponTracking(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();

    Model::Node* playerWeponFront = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandIndex1");
    Model::Node* playerWeponBack = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandMiddle1");

    DirectX::XMFLOAT3 playerWeponFrontPosition;
    playerWeponFrontPosition.x = playerWeponFront->worldTransform._41;
    playerWeponFrontPosition.y = playerWeponFront->worldTransform._42;
    playerWeponFrontPosition.z = playerWeponFront->worldTransform._43;

    DirectX::XMFLOAT3 playerWeponBackPosition;
    playerWeponBackPosition.x = playerWeponBack->worldTransform._41;
    playerWeponBackPosition.y = playerWeponBack->worldTransform._42;
    playerWeponBackPosition.z = playerWeponBack->worldTransform._43;

    DirectX::XMFLOAT3 weaponVector = {};
    weaponVector = Mathf::SubtractFloat3(playerWeponFrontPosition, playerWeponBackPosition);

    DirectX::XMVECTOR playerV;
    playerV = DirectX::XMLoadFloat3(&weaponVector);
    playerV = DirectX::XMVector3Normalize(playerV);
    DirectX::XMFLOAT3 insectPosition = {};
    DirectX::XMStoreFloat3(&weaponVector, playerV);

    insectPosition = Mathf::MultiplyFloat3(weaponVector, DirectX::XMFLOAT3(1, 1, 1));

    insectPosition = Mathf::MultiplyFloat3(insectPosition, position);

    weaponPosition = playerWeponFrontPosition;
    position = insectPosition;
    position = playerWeponFrontPosition;
}