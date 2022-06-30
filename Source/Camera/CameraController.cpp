#include "CameraController.h"
#include "Camera.h"
#include "Input\Input.h"
#include <imgui.h>
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "Mathf.h"

void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float padRX = gamePad.GetAxisRX();
    float padRY = gamePad.GetAxisRY();

    float speed = rollSpeed * elapsedTime;

    // スティックの入力値に合わせてX軸とY軸を回転
    angle.x += padRY * speed;
    angle.y += padRX * speed;

    // X軸のカメラ回転を制限
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // X軸のカメラ回転を制限
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    // カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye  = LockOn();
    //eye.x = target.x - front.x * range;
    //eye.y = target.y - front.y * range;
    //eye.z = target.z - front.z * range;

    // カメラの視点と注視点を設定
    DirectX::XMFLOAT3 up = { 0, 1, 0 };
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(up));

    ////デュアルモニター対応
    //    //マルチディスプレイの各種情報取得では・・・
    //GetSystemMetrics(SM_CMONITORS);  //で、ディスプレイ台数取得
    //GetSystemMetrics(SM_XVIRTUALSCREEN);
    //GetSystemMetrics(SM_YVIRTUALSCREEN);
    //GetSystemMetrics(SM_CXVIRTUALSCREEN);
    //GetSystemMetrics(SM_CYVIRTUALSCREEN); //で、仮想画面全体のサイズ取得
    //MonitorFromPoint(p, MONITOR_DEFAULTTONEARESET); //で、領域に対する
    //    //ディスプレイのハンドル取得・・・
    //GetMonitorInfo(hMon, &mInfo);  //で、ディスプレイハンドルからサイズ
    //    //情報を取得。
}

DirectX::XMFLOAT3 CameraController::LockOn()
{
    // プレーヤーから一番近いエネミーを算出する(カメラ内かどうかは無視)

    PlayerManager& playerManager = PlayerManager::Instance();
    Player* playerOne = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    DirectX::XMFLOAT3 pos = {0, 0, 0};
    for (int i = 0; i < enemyCount; i++)
    {
        DirectX::XMFLOAT3 length =
            Mathf::SubtractFloat3(enemyManager.GetEnemy(i)->GetPosition(), playerOne->GetPosition());
        float sq = sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
        DirectX::XMFLOAT3 unitvec_player_to_target = DirectX::XMFLOAT3(length.x / sq, length.y / sq, length.z / sq);

        //注視点取得
        DirectX::XMFLOAT3 target1 = DirectX::XMFLOAT3(
            enemyManager.GetEnemy(i)->GetPosition().x + unitvec_player_to_target.x,
            enemyManager.GetEnemy(i)->GetPosition().y + unitvec_player_to_target.y,
            enemyManager.GetEnemy(i)->GetPosition().z + unitvec_player_to_target.z);

        //カメラ位置取得
        pos = DirectX::XMFLOAT3(
            playerOne->GetPosition().x - unitvec_player_to_target.x * range, 1.0f,
            playerOne->GetPosition().z - unitvec_player_to_target.z * range);
    }

    return pos;
}

void CameraController::DrawDebugGUI()
{
   ImVec2 windowPosition = { 10, 10 };
   ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
   ImVec2 windowSize = { 300, 300 };
   ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
   // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);

    if (ImGui::Begin("CameraController", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 120), ImGuiWindowFlags_NoTitleBar);
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::LabelText(" ", "target (%.1f , %.1f , %.1f)", target.x, target.y, target.z);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // 回転速度
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // レンジ
            ImGui::SliderFloat("range", &range, 0.001f, 50.0f);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}