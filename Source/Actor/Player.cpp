#include <imgui.h>
#include "Input.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Collision.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "PlayerDerived.h"
#include "Extract.h"

Player::Player()
{
    model = std::make_shared<Model>("Data/Model/Player/Player.mdl");
    scale.x = scale.y = scale.z = size;

    radius = 0.5f;
    height = 2.2f;

    maxHealth = 100.0f;
    health = maxHealth;

    stateMachine = new StateMachine();

    Extract::Instance().Initialize();

#pragma region ステート登録
    stateMachine->RegisterState(new PlayerState::IdleState(this));
    stateMachine->RegisterState(new PlayerState::NeglectState(this));
    stateMachine->RegisterState(new PlayerState::WalkState(this));
    stateMachine->RegisterState(new PlayerState::RunState(this));
    stateMachine->RegisterState(new PlayerState::AttackCombo1State(this));
    stateMachine->RegisterState(new PlayerState::AttackCombo2State(this));
    stateMachine->RegisterState(new PlayerState::AttackCombo3State(this));
    stateMachine->RegisterState(new PlayerState::AttackDashuState(this));
    stateMachine->RegisterState(new PlayerState::AvoiDanceState(this));
    stateMachine->RegisterState(new PlayerState::DamagesState(this));
    stateMachine->RegisterState(new PlayerState::DieState(this));
#pragma endregion

    stateMachine->SetState(State::Idle);
}

Player::~Player()
{

}

void Player::Update(float elapsedTime)
{
    if (movingFlag)
    {
        // 進行ベクトル取得
        DirectX::XMFLOAT3 moveVec = GetMoveVec();
        float moveSpeed = this->moveSpeed * elapsedTime;
        if (Extract::Instance().GetExtract(ExtractColor::White))
            moveSpeed *= 1.2f;
        position.x += moveVec.x * moveSpeed;
        position.z += moveVec.z * moveSpeed;

        InputMove(elapsedTime);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if ((gamePad.GetButtonDown() & gamePad.KEY_Z) || position.y <= -10)
        position = { 0, 10, 0 };

    HealthMax();

    Extract::Instance().Update(elapsedTime);

    CollisionPlayerVsEnemies();

    stateMachine->Update(elapsedTime);

    // 速力処理更新
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTime(elapsedTime);

    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    model->RootMotion("mixamorig:Hips");

    model->UpdateTransform(transform);
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    float moveVecLength = DirectX::XMVectorGetX(
        DirectX::XMVector3Length(DirectX::XMLoadFloat3(&moveVec)));

    moveFlag = moveVecLength;
}

DirectX::XMFLOAT3 Player::GetMoveVec()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 vec;

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
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

    vec.y = 0.0f;
    return vec;
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
    {
        for (int j = 0; j < enemyManager.GetEnemy(i)->GetParts().size(); j++)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);

            DirectX::XMFLOAT3 outPosition;
            DirectX::XMFLOAT3 enemyPosition =
                enemy->GetNodePosition(enemy->GetNode(enemy->GetParts()[j].name));
            // そのままでは大きかったため半分に
            enemyPosition.y = enemyPosition.y - (enemy->GetParts()[j].radius / 2);
            if (Collision::IntersectCylinderVsNodeCylinder(
                position, radius, height,
                enemyPosition, enemy->GetParts()[j].radius, enemy->GetParts()[j].radius,
                outPosition))
            {
                position = outPosition;
            }
        }
    }
}

void Player::Render(RenderContext rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}

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
       // ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 120), ImGuiWindowFlags_NoTitleBar);

        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
            char* str = {};
            switch (GetStateMachine()->GetStateIndex())
            {
            case static_cast<int>(State::Idle):
                str = "Idle";
                break;
            case static_cast<int>(State::Neglect):
                str = "Neglect";
                break;
            case static_cast<int>(State::Walk):
                str = "Walk";
                break;
            case static_cast<int>(State::Run):
                str = "Run";
                break;
            case static_cast<int>(State::AttackCombo1):
                str = "AttackCombo1";
                break;
            case static_cast<int>(State::AttackCombo2):
                str = "AttackCombo2";
                break;
            case static_cast<int>(State::AttackCombo3):
                str = "AttackCombo3";
                break;
            case static_cast<int>(State::AttackDashu):
                str = "AttackDashu";
                break;
            case static_cast<int>(State::Avoidance):
                str = "Avoidance";
                break;
            case static_cast<int>(State::Damages):
                str = "Damages";
                break;
            case static_cast<int>(State::Die):
                str = "Die";
                break;
            }
            ImGui::Text("NowState %s", str);
        }

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Postion", &position.x, 0.1f);
            if (ImGui::Button("FallRescue"))
            {
                position = { 0, 5, 0 };
            }

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
        if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("Radius", &radius, 0.0005f, 0, 5);
            ImGui::DragFloat("Height", &height, 0.0005f, 0, 5);
        }
        ImGui::SliderFloat("Health", &health, 0.0f, maxHealth);
        if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            float animationSeconds = model->GetAnimationSeconds();
            ImGui::SliderFloat("animationSeconds", &animationSeconds, 0.0f, 3.0f);

            int i = static_cast<int>(movingFlag);
            ImGui::InputInt("movingFlag", &i);
        }
        ImGui::InputInt("extractColor", &extractColor);
        //ImGui::EndChild();
    }
    ImGui::End();
}

void Player::OnDamaged()
{
    stateMachine->ChangeState(State::Damages);
}

void Player::OnDead()
{
    stateMachine->ChangeState(State::Die);
}