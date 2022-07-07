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

    Mouse& mouse = Input::Instance().GetMouse();

    DirectX::XMFLOAT3 perspective;
    // ロックオンのオンオフ
    if ((mouse.GetButtonDown() & Mouse::BTN_MIDDLE ) && lockOnTimer > 0.2f /*||
         (gamePad.GetButton() & GamePad::BTN_RIGHT_THUMB)*/)
    {
        lockOnFlag = !lockOnFlag;
        lockOnTimer = 0.0f;
    }
    lockOnTimer += elapsedTime;

    if (lockOnFlag)
        perspective = LockOn(elapsedTime);
    else
    {
        // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
        perspective = GetPerspective();
        targetIndex = -1;
    }

    // カメラの視点と注視点を設定
    DirectX::XMFLOAT3 up = { 0, 1, 0 };
    Camera::Instance().SetLookAt(perspective, target, DirectX::XMFLOAT3(up));

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

DirectX::XMFLOAT3 CameraController::LockOn(float elapsedTime)
{
    // プレーヤーから一番近いエネミーを算出する(カメラ内かどうかは無視)

    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();

    const DirectX::XMFLOAT3 playerFront = player->GetFront();
    float min = 0;

    DirectX::XMFLOAT3 cameraPos = GetPerspective();
    for (int i = 0; i < enemyCount; i++)
    {
        DirectX::XMFLOAT3 length =
            Mathf::SubtractFloat3(enemyManager.GetEnemy(i)->GetPosition(), player->GetPosition());
        float square = sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
        DirectX::XMFLOAT3 playerEnemyLength = DirectX::XMFLOAT3(length.x / square, length.y / square, length.z / square);
        float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;

        if (min == 0 || min > playerEnemyLengthTotal)
        {
            min = playerEnemyLengthTotal;
            cameraPos = DirectX::XMFLOAT3(
                player->GetPosition().x - playerEnemyLength.x * playerRange,
                1.0f,
                player->GetPosition().z - playerEnemyLength.z * playerRange);
            targetIndex = i;
            lockOnFlag = true;
        }
    }

    if (min == 0)
    {
        // カメラをプレイヤーの正面へ向ける
        ResetCamera(elapsedTime);
        lockOnFlag = false;
    }
    return cameraPos;
}

DirectX::XMFLOAT3 CameraController::ResetCamera(float elapsedTime)
{
    // カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    DirectX::XMFLOAT3 perspective;
    perspective.x = target.x - front.x * playerRange;
    perspective.y = target.y - front.y * playerRange;
    perspective.z = target.z - front.z * playerRange;

    return perspective;
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
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::LabelText(" ", "target (%.1f , %.1f , %.1f)", target.x, target.y, target.z);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, -181, 181);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // 回転速度
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // レンジ
            ImGui::SliderFloat("playerRange", &playerRange, 0.001f, 50.0f);
        }
        if (ImGui::CollapsingHeader("Target", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("targetIndex  %d", targetIndex);
            Mouse& mouse = Input::Instance().GetMouse();
            ImGui::Text("holdDown %d", mouse.GetHoldDown());
            ImGui::SliderFloat("lockOnTimer", &lockOnTimer, 0, 1000);
        }
    }
    ImGui::End();
}

DirectX::XMFLOAT3 CameraController::GetPerspective()
{
    // カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    DirectX::XMFLOAT3 perspective;
    perspective.x = target.x - front.x * playerRange;
    perspective.y = target.y - front.y * playerRange;
    perspective.z = target.z - front.z * playerRange;

    return perspective;
}