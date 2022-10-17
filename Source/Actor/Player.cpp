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

    stateMachine->RegisterState(new ActionState(this));
    //stateMachine->RegisterState(new BattleState(this));
    // 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
    stateMachine->RegisterSubState(static_cast<int>(Player::State::Action), new IdleState(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Action), new NeglectState(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Action), new WalkState(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Action), new RunState(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Battle), new AttackCombo1State(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Battle), new AttackCombo2State(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Battle), new AttackCombo3State(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Battle), new AttackDashuState(this));
    //stateMachine->RegisterSubState(static_cast<int>(Player::State::Battle), new AvoiDanceState(this));
    // ステートをセット
    stateMachine->SetState(static_cast<int>(State::Action));
}

Player::~Player()
{
    delete model;
}

void Player::Update(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    float moveSpeed = this->moveSpeed * elapsedTime;
    position.x += moveVec.x * moveSpeed;
    position.z += moveVec.z * moveSpeed;

    InputMove(elapsedTime);

    InputAttack(elapsedTime);

    CollisionPlayerVsEnemies();

    CollisionNodeVsEnemies("mixamorig:Sword_joint", 0.5f);

    // 速力処理更新
    UpdateVelocity(elapsedTime);

    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);

    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // 進行ベクトルがゼロベクトルでない場合はスティックで移動中
    float moveVecLength =
        DirectX::XMVectorGetX(
            DirectX::XMVector3Length(
                DirectX::XMLoadFloat3(&moveVec)));

    if (moveVecLength > 0.0f)
        moveNow = true;
    else
        moveNow = false;
}

// スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
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

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

    Model::Node* node = model->FindNode("mixamorig:Sword_joint");

    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    debugRenderer->DrawSphere(nodePosition, 0.5f, DirectX::XMFLOAT4(1, 0, 0, 1));
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
        //ImGui::EndChild();
    }
    ImGui::End();
}