#include "CameraController.h"
#include "Camera.h"
#include "Input\Input.h"
#include <imgui.h>
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "StageManager.h"
#include "Mathf.h"
#include <algorithm>

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();

    // C�ŃJ�����̑�����}�E�X�ƃL�[�{�[�h������ւ�
    if (gamePad.GetButtonDown() & gamePad.KEY_C && lockOnTimer > 0.5f)
    {
        cameraMouseOperationFlag = !cameraMouseOperationFlag;
        lockOnTimer = 0.0f;
    }

    beforePerspective = perspective;

    switch (cameraMode)
    {
    case CameraMode::NormalCamera:
        UpdateNormalCamera(elapsedTime);
        break;
    case CameraMode::LockOnCamera:
        UpdateLockOnCamera(elapsedTime);
        break;
    }

    if (lerpFlag)
        UpdateTransitionCamera(elapsedTime);

    if (shakeFlag)
    {
        ShakeCamera(shakePower);
    }

    if (lerpFlag == false)
        perspective = afterPerspective;

    // �}�E�X�̃z�C���������݂�������Q�{�^�����E�X�e�B�b�N���������܂ꂽ�烍�b�N�I���̃I���I�t
    if ((mouse.GetButtonDown() & mouse.BTN_MIDDLE ||
        gamePad.GetButtonDown() & gamePad.BTN_RIGHT_THUMB) &&
        lockOnTimer > 0.5f)
    {
        lockOnFlag = !lockOnFlag;
        lockOnTimer = 0.0f;
        if (lockOnFlag)
            LockOn(elapsedTime);
        else if (!lockOnFlag)
        {
            cameraMode = CameraMode::NormalCamera;
            lerpFlag = true;
        }
    }
    if (lockOnTimer <= 0.5f)
        lockOnTimer += elapsedTime;

    // �X�e�[�W�Ƃ̃��C�L���X�g
    //UpdateStageRayCast();

    // �J�����̎��_�ƒ����_��ݒ�
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
            // �X�e�B�b�N�A�L�[�{�[�h���쎞�̉�]���x
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // �}�E�X���쎞�̉�]���x
            ImGui::SliderFloat("mouseRollSpeed", &mouseRollSpeed, 0.0f, 5.0f);
            // �����W
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
            ImGui::SliderFloat("lockOnTimer", &lockOnTimer, 0.0f, 0.5f);
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
            if (!shakeFlag)
            {
                if (ImGui::Button("shakeStart!"))
                    shakeFlag = !shakeFlag;
            }
            else
            {
                if (ImGui::Button("shakeStop!"))
                    shakeFlag = !shakeFlag;
            }
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

    // �}�E�X�J�[�\����\��
    // TODO ���̓J�[�\���̓��������邽�ߎ��s���Ȃ��A�{�Ԃł̓R�����g���O��
    //ShowCursor(false);

    float speed = mouseRollSpeed * elapsedTime;
    angle.x += (mouse.GetPositionY() - mouse.GetScreenHeight() / 2.0f) * speed;
    angle.y += (mouse.GetPositionX() - mouse.GetScreenWidth() / 2.0f) * speed;
}

void CameraController::UpdatePad(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float speed = rollSpeed * elapsedTime;
    float padRX = gamePad.GetAxisRX();
    float padRY = gamePad.GetAxisRY();
    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x -= padRY * speed;
    angle.y += padRX * speed;
}

void CameraController::UpdateKeyboard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // �}�E�X�J�[�\���\��
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
    // X���̃J������]�𐧌�
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // y���̃J������]�𐧌�
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
    // �J�����̃}�E�X����
    if (cameraMouseOperationFlag)
    {
        UpdateMouse(elapsedTime);
    }
#if _DEBUG
    // TODO �{�Ԃł̓}�E�X����̂��ߊJ�����̂ݎ��s����
    // �J�����̃L�[�{�[�hIJKL����
    else if (!cameraMouseOperationFlag)
    {
        UpdateKeyboard(elapsedTime);
    }
#endif

    // �J�����̉E�X�e�B�b�N����
    UpdatePad(elapsedTime);

    CameraRotationAxisLimit();

    afterPerspective = GetPerspective();
}

void CameraController::UpdateLockOnCamera(float elapsedTime)
{
    // �v���[���[�����ԋ߂��G�l�~�[���Z�o���� TODO (�J���������ǂ����͖���)
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();

    if (!lerpFlag)
    {
        if (LockOnSwitching()) {};
    }

    char* targetName =
        enemyManager.GetEnemy(0)->GetParts()[targets[nowTargetIndex].index].name;
    Model::Node* node =
        enemyManager.GetEnemy(0)->GetModel()->FindNode(targetName);
    DirectX::XMFLOAT3 targetposition = enemyManager.GetEnemy(0)->GetNodePosition(node);

    DirectX::XMFLOAT3 playerEnemyLength =
        Mathf::CalculateLength(targetposition, player->GetPosition());

    afterPerspective = DirectX::XMFLOAT3(
        player->GetPosition().x - playerEnemyLength.x * playerRange,
        player->GetPosition().y - playerEnemyLength.y + 1,
        player->GetPosition().z - playerEnemyLength.z * playerRange);

    //perspective = afterPerspective;
}

void CameraController::UpdateTransitionCamera(float elapsedTime)
{
    perspective = Mathf::LerpFloat3(perspective, afterPerspective, lerpTimer);
    lerpTimer += elapsedTime * lerpSpeed;

    if (lerpTimer > 1.0f)
    {
        lerpFlag = false;
        lerpTimer = 0.0f;
    }
}

void CameraController::UpdateStageRayCast()
{
    HitResult hitResult;
    StageManager& stageManager = StageManager::Instance();
    if (stageManager.GetStage(stageManager.GetNowStage())->
        RayCast(beforePerspective, afterPerspective, hitResult))
    {
        DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&hitResult.position);
        DirectX::XMVECTOR cuv = DirectX::XMVectorSet(0, 1, 0, 0);
        p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
        DirectX::XMStoreFloat3(&perspective, p);
        perspective = hitResult.position;
    }
}

void CameraController::LockOn(float elapsedTime)
{
    // �O�̏��͕K�v�������߃��Z�b�g
    targets.clear();
    nowTargetIndex = 0;

    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();

    // �v���C���[�̑O����(TODO �O�����Ƃ�ĂȂ�����)
    //const DirectX::XMFLOAT3 playerFront = player->GetFront();

    Target target;
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        for (int j = 0; j < enemy->GetParts().size(); j++)
        {
            if (enemy->GetParts()[j].cameraTargetFlag)
            {
                Model::Node* node = enemy->GetModel()->FindNode(enemy->GetParts()[j].name);
                DirectX::XMFLOAT3 playerEnemyLength =
                    Mathf::CalculateLength(enemy->GetNodePosition(node), player->GetPosition());
                float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;

                if (playerEnemyLengthTotal > lockOnRange) continue;

                target.enemyLengthTotal = playerEnemyLengthTotal;
                target.index = j;
                targets.emplace_back(target);
            }
        }
        DirectX::XMFLOAT3 playerEnemyLength =
            Mathf::CalculateLength(
                enemyManager.GetEnemy(i)->GetPosition(), player->GetPosition());
        float playerEnemyLengthTotal =
            playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;
    }

    // �Ώۂ����Ȃ������ꍇ
    if (!targets.size())
    {
        // �J�������v���C���[�̐��ʂ֌�����
        target.index = 0;
        perspective = ResetCamera(elapsedTime);
    }

    // ���������ɕ��בւ�
    std::sort(targets.begin(), targets.end());

    // �J�ڃX�e�[�g�ֈړ�
    cameraMode = CameraMode::LockOnCamera;
    lerpFlag = true;
}

// �J�������v���[���[�̐��ʂ֌�����
DirectX::XMFLOAT3 CameraController::ResetCamera(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    // �v���C���[�̑O����(TODO �O�����Ƃ�ĂȂ�����)
    const DirectX::XMFLOAT3 playerFront = player->GetFront();
    DirectX::XMVECTOR playerFrontVec = DirectX::XMLoadFloat3(&playerFront);
    // �^�[�Q�b�g
    DirectX::XMVECTOR targetVec = DirectX::XMVectorScale(playerFrontVec, 10);
    DirectX::XMFLOAT3 target;
    DirectX::XMStoreFloat3(&target, targetVec);

    return target;

    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    DirectX::XMFLOAT3 perspective;
    perspective.x = target.x - playerFront.x * playerRange;
    perspective.y = target.y - playerFront.y * playerRange;
    perspective.z = target.z - playerFront.z * playerRange;

    return perspective;
}

// ���b�N�I���ΏەύX
bool CameraController::LockOnSwitching()
{
    Mouse& mouse = Input::Instance().GetMouse();

    if (cameraMouseOperationFlag)
    {
        float mousePos = static_cast<float>(mouse.GetPositionX());
        // �}�E�X�̈ʒu���X�N���[���̐^�񒆂��獶�Ɉړ�������
        if (mousePos > mouse.GetScreenWidth() * 0.5f + 50)
        {
            Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
            // targets.index�̖�������Ȃ���ΑΏۂ�ύX
            if (nowTargetIndex < enemy->GetPartsCameraTargetFlagTotal() - 1)
            {
                // �J�ڃX�e�[�g�ֈړ�
                lerpFlag = true;
                nowTargetIndex++;
                return true;
            }
        }
        // �}�E�X�̈ʒu���X�N���[���̐^�񒆂���E�Ɉړ�������
        else if (mousePos < mouse.GetScreenWidth() * 0.5 - 50)
        {
            // targets.index�̐擪����Ȃ���ΑΏۂ�ύX
            if (nowTargetIndex  > 0)
            {
                // �J�ڃX�e�[�g�ֈړ�
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
    return false;
}

DirectX::XMFLOAT3 CameraController::GetPerspective()
{
    DirectX::XMFLOAT3 outPerspective;

    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    outPerspective.x = target.x - front.x * playerRange;
    outPerspective.y = target.y - front.y * playerRange;
    outPerspective.z = target.z - front.z * playerRange;

    return outPerspective;
}

// �J�����V�F�C�N
void CameraController::ShakeCamera(DirectX::XMFLOAT3 shakePower)
{
    shakePower.x = rand() % static_cast<int>(shakePower.x) * shakesuppress;
    shakePower.y = rand() % static_cast<int>(shakePower.y) * shakesuppress;
    shakePower.z = rand() % static_cast<int>(shakePower.z) * shakesuppress;

    // �V�F�C�N���̉��Z
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