#include "CameraController.h"
#include "Camera.h"
#include "Input\Input.h"
#include <imgui.h>
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "StageManager.h"
#include "Mathf.h"
#include <algorithm>


#include "EnemyBoss.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();
    // カメラのマウス操作
    if (cameraMouseOperationFlag)
    {
        UpdateMouse(elapsedTime);
    }
#if _DEBUG
    // TODO 本番ではマウス操作のため開発中のみ実行する
    // カメラのキーボードIJKL操作
    else if (!cameraMouseOperationFlag)
    {
        UpdateKeyboard(elapsedTime);
    }
#endif // DEBUG

    // カメラの右スティック操作
    UpdatePad(elapsedTime);

    // Cでカメラの操作をマウスとキーボードか入れ替え
    if (gamePad.GetButtonDown() & gamePad.KEY_C && lockOnTimer > 0.5f)
    {
        cameraMouseOperationFlag = !cameraMouseOperationFlag;
        lockOnTimer = 0.0f;
    }

    CameraRotationAxisLimit();

    switch (cameraMode)
    {
    case CameraMode::NormalCamera:
        //perspective = GetPerspective();
        UpdateNormalCamera(elapsedTime);
        break;
    case CameraMode::LockOnCamera:
        GetTargetPerspective();
        //UpdateLockOnCamera(elapsedTime);
        break;
    }

    if (lerpFlag)
    {
        perspective = UpdateTransitionState(elapsedTime);
    }

    // マウスのホイル押し込みもしくはQボタンか右スティックが押し込まれたらロックオンのオンオフ
    if ((mouse.GetButtonDown() & mouse.BTN_MIDDLE ||
        gamePad.GetButtonDown() & gamePad.BTN_RIGHT_THUMB) &&
        lockOnTimer > 0.5f)
    {
        lockOnFlag = !lockOnFlag;
        lockOnTimer = 0.0f;
        if (lockOnFlag)
            LockOn(elapsedTime);
        else if (!lockOnFlag)
            cameraMode = CameraMode::NormalCamera;
    }
    lockOnTimer += elapsedTime;

    ShakeCamera(shakePower);

    // 壁や床とのレイキャスト
    HitResult	hitResult;
    StageManager& stageManager = StageManager::Instance();
    if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(perspective, perspectiveq, hitResult))
    {

    }

    // カメラの視点と注視点を設定
    DirectX::XMFLOAT3 up = { 0, 1, 0 };
    Camera::Instance().SetLookAt(perspective, target, DirectX::XMFLOAT3(up));
}

void CameraController::DrawDebugPrimitive()
{

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
            // スティック、キーボード操作時の回転速度
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // マウス操作時の回転速度
            ImGui::SliderFloat("mouseRollSpeed", &mouseRollSpeed, 0.0f, 5.0f);
            // レンジ
            ImGui::SliderFloat("playerRange", &playerRange, 0.001f, 50.0f);
        }
        if (ImGui::CollapsingHeader("LockOn", ImGuiTreeNodeFlags_DefaultOpen))
        {
            {
                char* str = lockOnFlag ? "true" : "false";
                ImGui::Text("lockOnFlag %s", str);
            }
            ImGui::Text("nowTargetIndex  %d", nowTargetIndex);
            {
                char* str = {};
                switch (cameraMode)
                {
                case CameraMode::NormalCamera:
                    str = "NormalCamera";
                    break;
                case CameraMode::LockOnCamera:
                    str = "LockOnCamera";
                    break;
                }
                ImGui::Text("cameraMode %s", str);
            }
            ImGui::SliderFloat("lockOnPossitionY", &lockOnPositionY, -10.0f, 10.0f);
            Mouse& mouse = Input::Instance().GetMouse();
            ImGui::Text("holdDown %d", mouse.GetHoldDown());
            ImGui::SliderFloat("lockOnTimer", &lockOnTimer, 0, 1000);
            for (int i = 0; i < targets.size(); i++)
            {
                ImGui::Text("targetsX %d", targets[i].position.x);
                ImGui::Text("targetsY %d", targets[i].position.y);
                ImGui::Text("targetsZ %d", targets[i].position.z);
            }
        }
        ImGui::SliderFloat("near", &nearCamera, 0.0f, 10.0f);
        ImGui::SliderFloat("far", &farCamera, 0.0f, 10.0f);
        if (cameraMouseOperationFlag)
        {
            if (ImGui::Button("KeyboardOperation"))
                cameraMouseOperationFlag = false;
        }
        else
        {
            if (ImGui::Button("MouseOperation"))
                cameraMouseOperationFlag = true;
        }
        ImGui::SliderFloat("lockOnRange", &lockOnRange, 0.0f, 10.0f);
        if (ImGui::CollapsingHeader("CameraShake", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat3("shakePower", &shakePower.x, 1.0f, 10.0f);
            ImGui::SliderFloat("shakesuppress", &shakesuppress, 0.0f, 1.0f);
        }
        if (ImGui::CollapsingHeader("Learp", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat("lerpSpeed", &lerpSpeed, 0.0f, 10.0f);
            ImGui::SliderFloat("lerpTimer", &lerpTimer, 0.0f, 1.0f);
        }
    }
    ImGui::End();
}

void CameraController::UpdateMouse(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();

    // マウスカーソル非表示
    // TODO 今はカーソルの動きを見るため実行しない、本番ではコメントを外す
    //ShowCursor(false);

    float speed = mouseRollSpeed * elapsedTime;
    angle.x += (mouse.GetPositionY() - mouse.GetScreenHeight() / 2.0f) * speed;
    angle.y += (mouse.GetPositionX() - mouse.GetScreenWidth() / 2.0f) * speed;

    mouse.SetMiddlePosition();
}

void CameraController::UpdatePad(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float speed = rollSpeed * elapsedTime;
    float padRX = gamePad.GetAxisRX();
    float padRY = gamePad.GetAxisRY();
    // スティックの入力値に合わせてX軸とY軸を回転
    angle.x -= padRY * speed;
    angle.y += padRX * speed;
}

void CameraController::UpdateKeyboard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // マウスカーソル表示
    ShowCursor(true);

    float speed = rollSpeed * elapsedTime;

    if (gamePad.GetButton() & gamePad.KEY_I)
        angle.x -= 1.0f * speed;
    if (gamePad.GetButton() & gamePad.KEY_K)
        angle.x += 1.0f * speed;
    if (gamePad.GetButton() & gamePad.KEY_J)
        angle.y -= 1.0f * speed;
    if (gamePad.GetButton() & gamePad.KEY_L)
        angle.y += 1.0f * speed;
}

void CameraController::CameraRotationAxisLimit()
{
    // X軸のカメラ回転を制限
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // y軸のカメラ回転を制限
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
}

void CameraController::UpdateNormalCamera(float elapsedTime)
{
    perspective = GetPerspective();
}

void CameraController::UpdateLockOnCamera(float elapsedTime)
{
    ////	後方斜に移動させる
    //DirectX::XMVECTOR	t0 = DirectX::XMVectorSet(targetWork[0].x, 0.5f, targetWork[0].z, 0);
    //DirectX::XMVECTOR	t1 = DirectX::XMVectorSet(targetWork[1].x, 0.5f, targetWork[1].z, 0);
    //DirectX::XMVECTOR	crv = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
    //DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
    //DirectX::XMVECTOR	v = DirectX::XMVectorSubtract(t1, t0);
    //DirectX::XMVECTOR	l = DirectX::XMVector3Length(v);

    //t0 = DirectX::XMLoadFloat3(&targetWork[0]);
    //t1 = DirectX::XMLoadFloat3(&targetWork[1]);

    ////	新しい注視点を算出
    //DirectX::XMStoreFloat3(&newTarget, DirectX::XMVectorMultiplyAdd(v, DirectX::XMVectorReplicate(0.5f), t0));

    ////	新しい座標を算出
    //l = DirectX::XMVectorClamp(l
    //    , DirectX::XMVectorReplicate(lengthLimit[0])
    //    , DirectX::XMVectorReplicate(lengthLimit[1]));
    //t0 = DirectX::XMVectorMultiplyAdd(l, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), t0);
    //t0 = DirectX::XMVectorMultiplyAdd(crv, DirectX::XMVectorReplicate(sideValue * 3.0f), t0);
    //t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);
    //DirectX::XMStoreFloat3(&newPosition, t0);
}

void CameraController::LockOn(float elapsedTime)
{
    // 前の情報は必要無いためリセット
    targets.clear();
    nowTargetIndex = 0;

    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();

    // プレイヤーの前方向(TODO 前方向とれてないかも)
    //const DirectX::XMFLOAT3 playerFront = player->GetFront();

    DirectX::XMFLOAT3 cameraPos = perspective;

    Target target;
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        for (int j = 0; j < enemy->GetParts().size(); j++)
        {
            if (enemy->GetParts()[j].cameraTargetFlag == true)
            {
                Model::Node* node = enemy->GetModel()->FindNode(enemy->GetParts()[j].name);
                DirectX::XMFLOAT3 playerEnemyLength =
                    Mathf::CalculateLength(enemy->GetNodePosition(node), player->GetPosition());
                float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;

                if (playerEnemyLengthTotal > lockOnRange) continue;

                target.enemyLengthTotal = playerEnemyLengthTotal;
                target.index = j;
                target.position = enemy->GetNodePosition(node);
                targets.emplace_back(target);
            }
        }
        DirectX::XMFLOAT3 playerEnemyLength =
            Mathf::CalculateLength(enemyManager.GetEnemy(i)->GetPosition(), player->GetPosition());
        float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;


    }

    // 対象がいなかった場合
    if (!targets.size())
    {
        // カメラをプレイヤーの正面へ向ける
        target.index = 0;
        perspective = ResetCamera(elapsedTime);
    }

    // 小さい順に並べ替え
    std::sort(targets.begin(), targets.end());
    targets;

    // 遷移ステートへ移動
    lerpFlag = true;
}

// カメラをプレーヤーの正面へ向ける
DirectX::XMFLOAT3 CameraController::ResetCamera(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    // プレイヤーの前方向(TODO 前方向とれてないかも)
    const DirectX::XMFLOAT3 playerFront = player->GetFront();
    DirectX::XMVECTOR playerFrontVec = DirectX::XMLoadFloat3(&playerFront);
    // ターゲット
    DirectX::XMVECTOR targetVec = DirectX::XMVectorScale(playerFrontVec, 10);
    DirectX::XMFLOAT3 target;
    DirectX::XMStoreFloat3(&target, targetVec);

    //TODO 前方向とれてるかテスト、絶対消すこと
    {
        //EnemyManager& enemyManager = EnemyManager::Instance();

        //EnemyBoss* boss = new EnemyBoss();
        //boss->SetPosition(DirectX::XMFLOAT3(target));
        //boss->SetScale(DirectX::XMFLOAT3(1, 1000, 1));
        //enemyManager.Register(boss);
    }

    return target;

    // カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    DirectX::XMFLOAT3 perspective;
    perspective.x = target.x - playerFront.x * playerRange;
    perspective.y = target.y - playerFront.y * playerRange;
    perspective.z = target.z - playerFront.z * playerRange;

    return perspective;
}

// ロックオン対象変更
bool CameraController::LockOnSwitching()
{
    Mouse& mouse = Input::Instance().GetMouse();

    if (cameraMouseOperationFlag)
    {
        //float mousePos = static_cast<float>(mouse.GetPositionX() - mouse.GetOldPositionX());
        float mousePos = static_cast<float>(mouse.GetPositionX());
        // マウスの位置がスクリーンの真ん中から左に移動したら
        if (mousePos > mouse.GetScreenWidth() * 0.5f + 50)
        {
            Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
            // targets.indexの末尾じゃなければ対象を変更
            if (nowTargetIndex < /*!= targets.back().index*/ enemy->GetPartsCameraTargetFlagTotal() - 1)
            {
                // 遷移ステートへ移動
                lerpFlag = true;
                nowTargetIndex++;
                return true;
            }
        }
        // マウスの位置がスクリーンの真ん中から右に移動したら
        else if (mousePos < mouse.GetScreenWidth() * 0.5 - 50)
        {
            // targets.indexの先頭じゃなければ対象を変更
            if (nowTargetIndex  > /*!= targets.front().index*/ 0)
            {
                // 遷移ステートへ移動
                lerpFlag = true;
                nowTargetIndex--;
                return true;
            }
        }
    }
    return false;
}

bool CameraController::GetTargetPerspective()
{
    if (targets.size() <= 0)
        return false;

    // プレーヤーから一番近いエネミーを算出する TODO (カメラ内かどうかは無視)
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();

    if (!lerpFlag)
    {
        if (LockOnSwitching()) {};
    }
  /*  DirectX::XMFLOAT3 targetposition =
        enemyManager.GetEnemy(targets[nowTargetIndex].index)->GetPosition();
    targetposition.y +=
        enemyManager.GetEnemy(targets[nowTargetIndex].index)->GetHeight() / 2;*/

    DirectX::XMFLOAT3 targetposition = targets[nowTargetIndex].position;

    DirectX::XMFLOAT3 playerEnemyLength =
        Mathf::CalculateLength(targetposition, player->GetPosition());

    perspectiveq = DirectX::XMFLOAT3(
        player->GetPosition().x - playerEnemyLength.x * playerRange,
        player->GetPosition().y - playerEnemyLength.y + lockOnPositionY,
        player->GetPosition().z - playerEnemyLength.z * playerRange);

    perspective = perspectiveq;
    return true;
}

DirectX::XMFLOAT3 CameraController::UpdateTransitionState(float elapsedTime)
{
    DirectX::XMFLOAT3 interpolation = {};
    interpolation.x = Mathf::LerpFloat(perspective.x, perspectiveq.x, lerpTimer);
    interpolation.y = Mathf::LerpFloat(perspective.y, perspectiveq.y, lerpTimer);
    interpolation.z = Mathf::LerpFloat(perspective.z, perspectiveq.z, lerpTimer);

    lerpTimer += elapsedTime * lerpSpeed;

    if (lerpTimer > 1.0f)
    {
        cameraMode = CameraMode::LockOnCamera;
        lerpTimer = 0.0f;
        lerpFlag = false;
    }
    return interpolation;
}

DirectX::XMFLOAT3 CameraController::GetPerspective()
{
    DirectX::XMFLOAT3 perspective;

    // カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    perspective.x = target.x - front.x * playerRange;
    perspective.y = target.y - front.y * playerRange;
    perspective.z = target.z - front.z * playerRange;

    return perspective;
}

// カメラシェイク
void CameraController::ShakeCamera(DirectX::XMFLOAT3 shakePower)
{
    shakePower.x = rand() % static_cast<int>(shakePower.x) * shakesuppress;
    shakePower.y = rand() % static_cast<int>(shakePower.y) * shakesuppress;
    shakePower.z = rand() % static_cast<int>(shakePower.z) * shakesuppress;

    // シェイク分の加算
    target.x += shakePower.x;
    target.y += shakePower.y;
    target.z += shakePower.z;
    perspective.x += shakePower.x;
    perspective.y += shakePower.y;
    perspective.z += shakePower.z;
}

void CameraController::SetCamerarShake(DirectX::XMFLOAT3 shakePower, float shakeTime)
{
    shakeTimer += 1.0f;
    if (shakeTime < shakeTimer)
    {
        ShakeCamera(shakePower);
    }
}