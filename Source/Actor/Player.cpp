#include <imgui.h>
#include "Input\Input.h"
#include "Graphics/Graphics.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "PlayerDerived.h"

Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");
    scale.x = scale.y = scale.z = size;

    radius = 0.5f;
    height = 2.0f;

    health = 1000.0f;

    stateMachine = new StateMachine();

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
#pragma endregion

    stateMachine->SetState(State::Idle);
}

Player::~Player()
{
    delete model;
}

void Player::Update(float elapsedTime)
{
    // �i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    float moveSpeed = this->moveSpeed * elapsedTime;
    position.x += moveVec.x * moveSpeed;
    position.z += moveVec.z * moveSpeed;

    InputMove(elapsedTime);

    InputAttack(elapsedTime);

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

// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

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
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

    vec.y = 0.0f;
    return vec;
}

DirectX::XMFLOAT3 Player::GetFront() const
{
    DirectX::XMFLOAT3 front;
    front.x = sinf(angle.y);
    front.y = 0.0f;
    front.z = cosf(angle.y);

    return front;
}


void Player::InputAttack(float elapsedTime)
{
    float SwordRadius = 0.5f;
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsSpherer(
            position, radius, enemy->GetPosition(),
            enemy->GetRadius(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
        }
    }
}

void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    Model::Node* node = model->FindNode(nodeName);

    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsSpherer(
            nodePosition, nodeRadius, enemy->GetPosition(),
            enemy->GetRadius(),
            outPosition))
        {
            enemy->SetPosition({ outPosition.x, 0, outPosition.z });
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
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {

        }
        //ImGui::EndChild();
    }
    ImGui::End();
}

void Player::OnDamaged()
{

}

void Player::OnDead()
{

}