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

    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x += padRY * speed;
    angle.y += padRX * speed;

    // X���̃J������]�𐧌�
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // X���̃J������]�𐧌�
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
    // ���b�N�I���̃I���I�t
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
        // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
        perspective = GetPerspective();
        targetIndex = -1;
    }

    // �J�����̎��_�ƒ����_��ݒ�
    DirectX::XMFLOAT3 up = { 0, 1, 0 };
    Camera::Instance().SetLookAt(perspective, target, DirectX::XMFLOAT3(up));

    ////�f���A�����j�^�[�Ή�
    //    //�}���`�f�B�X�v���C�̊e����擾�ł́E�E�E
    //GetSystemMetrics(SM_CMONITORS);  //�ŁA�f�B�X�v���C�䐔�擾
    //GetSystemMetrics(SM_XVIRTUALSCREEN);
    //GetSystemMetrics(SM_YVIRTUALSCREEN);
    //GetSystemMetrics(SM_CXVIRTUALSCREEN);
    //GetSystemMetrics(SM_CYVIRTUALSCREEN); //�ŁA���z��ʑS�̂̃T�C�Y�擾
    //MonitorFromPoint(p, MONITOR_DEFAULTTONEARESET); //�ŁA�̈�ɑ΂���
    //    //�f�B�X�v���C�̃n���h���擾�E�E�E
    //GetMonitorInfo(hMon, &mInfo);  //�ŁA�f�B�X�v���C�n���h������T�C�Y
    //    //�����擾�B
}

DirectX::XMFLOAT3 CameraController::LockOn(float elapsedTime)
{
    // �v���[���[�����ԋ߂��G�l�~�[���Z�o����(�J���������ǂ����͖���)

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
        // �J�������v���C���[�̐��ʂ֌�����
        ResetCamera(elapsedTime);
        lockOnFlag = false;
    }
    return cameraPos;
}

DirectX::XMFLOAT3 CameraController::ResetCamera(float elapsedTime)
{
    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
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
   // �E�B���h�E�̓����x
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);

    if (ImGui::Begin("CameraController", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::LabelText(" ", "target (%.1f , %.1f , %.1f)", target.x, target.y, target.z);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, -181, 181);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // ��]���x
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // �����W
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
    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    DirectX::XMFLOAT3 perspective;
    perspective.x = target.x - front.x * playerRange;
    perspective.y = target.y - front.y * playerRange;
    perspective.z = target.z - front.z * playerRange;

    return perspective;
}