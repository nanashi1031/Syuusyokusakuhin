#include "CameraController.h"
#include "Camera.h"
#include "Input\Input.h"
#include <imgui.h>
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "Mathf.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();
    // カメラのマウス操作
    if (cameraMouseOperationFlag)
    {
        UpdateMouse(elapsedTime);
    }
    // カメラのキーボードIJKL操作
    else if (!cameraMouseOperationFlag)
    {
        UpdateKeyboard(elapsedTime);
    }
    // カメラの右スティック操作
    UpdatePad(elapsedTime);

    if (gamePad.GetButtonDown() & gamePad.KEY_C && lockOnTimer > 0.5f)
    {
        cameraMouseOperationFlag = !cameraMouseOperationFlag;
        lockOnTimer = 0.0f;
    }

    CameraRotationAxisLimit();

    DirectX::XMFLOAT3 perspective;
    // ロックオンのオンオフ
    if ((mouse.GetButtonDown() & mouse.BTN_MIDDLE ||
        gamePad.GetButtonDown() & gamePad.BTN_RIGHT_THUMB) &&
        lockOnTimer > 0.5f)
    {
        lockOnFlag = !lockOnFlag;
        lockOnTimer = 0.0f;
    }
    lockOnTimer += elapsedTime;

    if (lockOnFlag)
    {
        perspective = LockOn(elapsedTime);
    }
    else
    {
        // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
        perspective = GetPerspective();
        targetIndex = -1;
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
        if (ImGui::CollapsingHeader("Target", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("targetIndex  %d", targetIndex);
            ImGui::SliderFloat("lockOnPossitionY", &lockOnPossitionY, -10.0f, 10.0f);
            Mouse& mouse = Input::Instance().GetMouse();
            ImGui::Text("holdDown %d", mouse.GetHoldDown());
            ImGui::SliderFloat("lockOnTimer", &lockOnTimer, 0, 1000);
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
    }
    ImGui::End();
}

void CameraController::UpdateMouse(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();

    // マウスカーソル非表示
    ShowCursor(false);

    float speed = mouseRollSpeed * elapsedTime;
    angle.x += (mouse.GetPositionY() - mouse.GetScreenHeight() * 0.5f) * speed;
    angle.y += (mouse.GetPositionX() - mouse.GetScreenWidth() * 0.5f) * speed;
    if (!(mouse.GetButtonDown() & mouse.BTN_RIGHT))
    {
        mouse.SetMiddlePosition();
    }
}

void CameraController::UpdatePad(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // マウスカーソル表示
    ShowCursor(true);

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

    // X軸のカメラ回転を制限
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
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
                player->GetPosition().y - playerEnemyLength.y + lockOnPossitionY,
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

void CameraController::CalculateFrustum(Plane* frustum)
{
    Camera& camera = Camera::Instance();
    DirectX::XMFLOAT3 cameraPos = camera.GetEye();

    // ビュープロジェクション行列を取得する
    DirectX::XMMATRIX matrix = {};
    DirectX::XMMATRIX viewMat = DirectX::XMLoadFloat4x4(&camera.GetView());
    DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(45),
        Graphics::Instance().GetScreenWidth() / Graphics::Instance().GetScreenHeight(),
        nearCamera, farCamera);
    matrix = viewMat * projMat;

    //ビュープロジェクション行列の逆行列
    DirectX::XMMATRIX invMatrix = DirectX::XMMatrixInverse(nullptr, matrix);
    //ビュープロジェクション内の頂点算出用位置ベクトル
    DirectX::XMVECTOR verts[8] =
    {
        // near plane corners
        { -1, -1, 0 },	// [0]:左下
        {  1, -1, 0 },	// [1]:右下
        {  1,  1, 0 },	// [2]:右上
        { -1,  1 ,0 },	// [3]:左上

        // far plane corners.
        { -1, -1, 1 },	// [4]:左下
        {  1, -1, 1 },	// [5]:右下
        {  1,  1, 1 },	// [6]:右上
        { -1,  1, 1 } 	// [7]:左上
    };

    // ビュープロジェクション行列の逆行列を用いて、各頂点を算出する
    for (int i = 0; i < 8; i++)
    {
        verts[i] = DirectX::XMVector3TransformCoord(verts[i], invMatrix);
        if (i < 4)
        {
            DirectX::XMStoreFloat3(&nearPoint[i], verts[i]);
        }
        else
        {
            DirectX::XMStoreFloat3(&farPoint[i - 4], verts[i]);
        }
    }

    DirectX::XMFLOAT4X4 matrix4X4 = {};
    DirectX::XMStoreFloat4x4(&matrix4X4, matrix);

    // 視錐台（フラスタム）を構成する６平面を算出する
    // 0:左側面, 1:右側面, 2:下側面, 3:上側面, 4:奥側面, 5:手前側面
    // 全ての面の法線は内側を向くように設定すること

    // 左側面
    frustum[0].normal.x = matrix4X4._14 + matrix4X4._11;
    frustum[0].normal.y = matrix4X4._24 + matrix4X4._21;
    frustum[0].normal.z = matrix4X4._34 + matrix4X4._31;
    DirectX::XMStoreFloat3(&frustum[0].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[0].normal)));
    frustum[0].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[0].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // 右側面
    frustum[1].normal.x = matrix4X4._14 - matrix4X4._11;
    frustum[1].normal.y = matrix4X4._24 - matrix4X4._21;
    frustum[1].normal.z = matrix4X4._34 - matrix4X4._31;
    DirectX::XMStoreFloat3(&frustum[1].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[1].normal)));
    frustum[1].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[1].normal), DirectX::XMLoadFloat3(&nearPoint[1])));

    // 下側面
    frustum[2].normal.x = matrix4X4._14 + matrix4X4._12;
    frustum[2].normal.y = matrix4X4._24 + matrix4X4._22;
    frustum[2].normal.z = matrix4X4._34 + matrix4X4._32;
    DirectX::XMStoreFloat3(&frustum[2].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[2].normal)));
    frustum[2].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[2].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // 上側面
    frustum[3].normal.x = matrix4X4._14 - matrix4X4._12;
    frustum[3].normal.y = matrix4X4._24 - matrix4X4._22;
    frustum[3].normal.z = matrix4X4._34 - matrix4X4._32;
    DirectX::XMStoreFloat3(&frustum[3].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[3].normal)));
    frustum[3].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[3].normal), DirectX::XMLoadFloat3(&nearPoint[2])));

    // 奥側面
    frustum[4].normal.x = -matrix4X4._14 - matrix4X4._13;
    frustum[4].normal.y = -matrix4X4._24 - matrix4X4._23;
    frustum[4].normal.z = -matrix4X4._34 - matrix4X4._33;
    DirectX::XMStoreFloat3(&frustum[4].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[4].normal)));
    frustum[4].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[4].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // 手前側面
    frustum[5].normal.x = -matrix4X4._14 + matrix4X4._13;
    frustum[5].normal.y = -matrix4X4._24 + matrix4X4._23;
    frustum[5].normal.z = -matrix4X4._34 + matrix4X4._33;
    DirectX::XMStoreFloat3(&frustum[5].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[5].normal)));
    frustum[5].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[5].normal), DirectX::XMLoadFloat3(&nearPoint[0])));
}

bool CameraController::frustumCulling(DirectX::XMFLOAT3 position, float radius)
{
    // 視錐台（フラスタム）との交差・内外判定
      // AABBが視錐台の内部にある			：collisionState = 0
      // AABBが視錐台の境界と交わっている	：collisionState = 1
      // AABBが視錐台の外部にある			：collisionState = 2

      // いったん内部にあるとしておく
    collisionState = 0;

    DirectX::XMFLOAT3 enemyPosition = position;
    float enemyRadius = radius;
    for (int j = 0; j < 6; j++)
    {
        // 各平面の法線の成分を用いてAABBの８頂点の中から最近点と最遠点を求める
        DirectX::XMFLOAT3 NegaPos = enemyPosition;	// 最近点
        DirectX::XMFLOAT3 PosiPos = enemyPosition;	// 最遠点

        if (frustum[j].normal.x > 0.0f)
        {
            NegaPos.x -= enemyRadius;
            PosiPos.x += enemyRadius;
        }
        else
        {
            NegaPos.x += enemyRadius;
            PosiPos.x -= enemyRadius;
        }
        if (frustum[j].normal.y > 0.0f)
        {
            NegaPos.y -= enemyRadius;
            PosiPos.y += enemyRadius;
        }
        else
        {
            NegaPos.y += enemyRadius;
            PosiPos.y -= enemyRadius;
        }
        if (frustum[j].normal.z > 0.0f)
        {
            NegaPos.z -= enemyRadius;
            PosiPos.z += enemyRadius;
        }
        else
        {
            NegaPos.z += enemyRadius;
            PosiPos.z -= enemyRadius;
        }

        // 各平面との内積を計算し、交差・内外判定を行う
        // 外部と分かれば処理をbreakし確定させる
        // 交差状態であれば、ステータスを変更してから次の平面とのチェックに続ける
        // 内部であれば、そのまま次の平面とのチェックに続ける
        float dotNegative = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[j].normal), DirectX::XMLoadFloat3(&NegaPos)));
        float dotPositive = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[j].normal), DirectX::XMLoadFloat3(&PosiPos)));

        if (dotNegative < frustum[j].distance && dotPositive < frustum[j].distance)
        {
            collisionState = 2;
            break;
        }
        else if ((dotNegative - frustum[j].distance) * (dotPositive - frustum[j].distance) < 0.0f)
        {
            collisionState = 1;
        }
    }

    if (collisionState == 0 || collisionState == 1)
    {
        // 敵の方向にレイを飛ばす
        DirectX::XMFLOAT3 toEnemy = { 0, 0, 0 };
        DirectX::XMStoreFloat3(&toEnemy, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&Camera::Instance().GetEye())));

        // レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { Camera::Instance().GetEye().x, Camera::Instance().GetEye().y + 0.5f, Camera::Instance().GetEye().z };
        DirectX::XMFLOAT3 end = { Camera::Instance().GetEye().x + toEnemy.x, Camera::Instance().GetEye().y + toEnemy.y + 0.5f, Camera::Instance().GetEye().z + toEnemy.z };

        return true;
    }

    return false;
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