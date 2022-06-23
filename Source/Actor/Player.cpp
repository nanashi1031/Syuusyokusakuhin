#include <imgui.h>
#include "Input\Input.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "Camera.h"

Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");
    model->PlayAnimation(1, true);
    scale.x = scale.y = scale.z = size;

    radius = 0.5f;
    height = 2.0f;

    health = 1000.0f;
}

Player::~Player()
{
    delete model;
}

void Player::Update(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    // 移動処理
    float moveSpeed = this->moveSpeed * elapsedTime;
    position.x += moveVec.x * moveSpeed;
    position.z += moveVec.z * moveSpeed;

    InputMove(elapsedTime);

    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImVec2 windowPosition = { 10, 10 };
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
    ImVec2 windowSize = { 300, 300 };
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);
    //ImGui::SetNextTreeNodeOpen();

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 120), ImGuiWindowFlags_NoTitleBar);
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::DragFloat3("Postion", &position.x, 0.1f);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
            ImGui::DragFloat3("Scale", &scale.x, 0.0005f, 0, 1000);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // 移動処理
    Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

    // 旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    // カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    // 移動ベクトルはXZ平面に水平なベクトルになるようにする
    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    // Y軸方向には移動しない
    vec.y = 0.0f;
    return vec;
}
