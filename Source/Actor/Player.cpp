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

#pragma region �X�e�[�g�o�^
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
        // �i�s�x�N�g���擾
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

    // ���͏����X�V
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTime(elapsedTime);

    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    model->RootMotion("mixamorig:Hips");

    model->UpdateTransform(transform);
}

// �ړ����͏���
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

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����
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
            // ���̂܂܂ł͑傫���������ߔ�����
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
    // �E�B���h�E�̓����x
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