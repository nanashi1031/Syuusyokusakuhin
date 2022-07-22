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
    // �J�����̃}�E�X����
    if (cameraMouseOperationFlag)
    {
        UpdateMouse(elapsedTime);
    }
#if _DEBUG
    // �J�����̃L�[�{�[�hIJKL����
    else if (!cameraMouseOperationFlag)
    {
        UpdateKeyboard(elapsedTime);
    }
#endif // DEBUG

    // �J�����̉E�X�e�B�b�N����
    UpdatePad(elapsedTime);

    // C�ŃJ�����̑�����}�E�X�ƃL�[�{�[�h������ւ�
    if (gamePad.GetButtonDown() & gamePad.KEY_C && lockOnTimer > 0.5f)
    {
        cameraMouseOperationFlag = !cameraMouseOperationFlag;
        lockOnTimer = 0.0f;
    }

    CameraRotationAxisLimit();

    switch (state)
    {
        // �J�����m�[�}�����
    case CameraContorollerState::NormalTargetState:
        perspective = GetPerspective();
        break;
        // �J�������b�N�I�����
    case CameraContorollerState::LockOnTargetState:
        GetTargetPerspective();
        break;
        // �J�����J�ڏ��
    case CameraContorollerState::TransitionState:
        perspective = UpdateTransitionState(elapsedTime);
        break;
    }

    // ���b�N�I���̃I���I�t
    if ((mouse.GetButtonDown() & mouse.BTN_MIDDLE ||
        gamePad.GetButtonDown() & gamePad.BTN_RIGHT_THUMB) &&
        lockOnTimer > 0.5f)
    {
        lockOnFlag = !lockOnFlag;
        lockOnTimer = 0.0f;
        if (lockOnFlag) LockOn(elapsedTime);
    }
    lockOnTimer += elapsedTime;

    ShakeCamera(shakePower);

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
        if (ImGui::CollapsingHeader("Target", ImGuiTreeNodeFlags_DefaultOpen))
        {
            {
                char* str = lockOnFlag ? "true" : "false";
                ImGui::Text("lockOnFlag %s", str);
            }
            ImGui::Text("nowTargetIndex  %d", nowTargetIndex);
            ImGui::SliderFloat("lockOnPossitionY", &lockOnPositionY, -10.0f, 10.0f);
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
        ImGui::SliderFloat("lockOnRange", &lockOnRange, 0.0f, 10.0f);
        if (ImGui::CollapsingHeader("CameraShake", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat3("shakePower", &shakePower.x, 1.0f, 10.0f);
            ImGui::SliderFloat("shakesuppress", &shakesuppress, 0.0f, 1.0f);
        }
    }
    ImGui::End();
}

void CameraController::UpdateMouse(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();

    // �}�E�X�J�[�\����\��
    // ���̓J�[�\���̓��������邽�ߎ��s���Ȃ��A�{�Ԃł̓R�����g���O��
    //ShowCursor(false);

    float speed = mouseRollSpeed * elapsedTime;
    angle.x += (mouse.GetPositionY() - mouse.GetScreenHeight() * 0.5f) * speed;
    angle.y += (mouse.GetPositionX() - mouse.GetScreenWidth() * 0.5f) * speed;

    mouse.SetMiddlePosition();
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

    // X���̃J������]�𐧌�
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
}

void CameraController::LockOn(float elapsedTime)
{
    // �J�ڃX�e�[�g�ֈړ�
    state = CameraContorollerState::TransitionState;

    // �O�̏��͕K�v�������߃��Z�b�g
    targets.clear();

    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount = enemyManager.GetEnemyCount();

    // �v���C���[�̑O����(�O�����Ƃ�ĂȂ�����)
    //const DirectX::XMFLOAT3 playerFront = player->GetFront();

    DirectX::XMFLOAT3 cameraPos = perspective;
    for (int i = 0; i < enemyCount; i++)
    {
        DirectX::XMFLOAT3 playerEnemyLength =
            Mathf::CalculateLength(enemyManager.GetEnemy(i)->GetPosition(), player->GetPosition());
        float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;

        if (playerEnemyLengthTotal > lockOnRange) continue;

        targets.emplace_back();
        Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());
    }

    if (!targets.size())
    {
        // �J�������v���C���[�̐��ʂ֌�����
        perspective = ResetCamera(elapsedTime);
        lockOnFlag = false;
        return;
    }

    // ���������ɕ��בւ�
    //targets.sort();
    auto list = targets.begin();
    for (int i = 1; i <= targets.size(); list++)
    {
        if (list == targets.end()) break;

        for (int j = 0; j < enemyCount; j++)
        {
            DirectX::XMFLOAT3 playerEnemyLength =
                Mathf::CalculateLength(enemyManager.GetEnemy(i)->GetPosition(), player->GetPosition());
            float playerEnemyLengthTotal = playerEnemyLength.x + playerEnemyLength.y + playerEnemyLength.z;

            //if (*list == playerEnemyLengthTotal)
            {
                // �v���C���[����߂����̃G�l�~�[�̔ԍ�
                //targets->push_back(j);
                break;
            }
        }
    }
}

auto CameraController::LockOnSwitching()
{
    Mouse& mouse = Input::Instance().GetMouse();
    auto itr = targets.begin();

    if (cameraMouseOperationFlag)
    {
        float mousePos = mouse.GetPositionX() - mouse.GetOldPositionX();
        // �E�����ɃX�N���[���̉��������100�������������ړ�������
        if (mousePos > mouse.GetScreenWidth() / 100)
        {
            // list�̍ő�l����Ȃ���ΑΏۂ�ύX
            if (itr != targets.end())
                // �J�ڃX�e�[�g�ֈړ�
                //state = CameraContorollerState::TransitionState;
                return itr++;
        }
        // �������ɃX�N���[���̉��������100�������������ړ�������
        else if (mousePos < -mouse.GetScreenWidth() / 100)
        {
            // list�̍ŏ��l����Ȃ���ΑΏۂ�ύX
            if (itr != targets.begin())
            {
                // �J�ڃX�e�[�g�ֈړ�
                //state = CameraContorollerState::TransitionState;
                return itr--;
            }
        }
    }
    return itr;
}

void CameraController::CalculateFrustum(Plane* frustum)
{
    Camera& camera = Camera::Instance();
    DirectX::XMFLOAT3 cameraPos = camera.GetEye();

    // �r���[�v���W�F�N�V�����s����擾����
    DirectX::XMMATRIX matrix = {};
    DirectX::XMMATRIX viewMat = DirectX::XMLoadFloat4x4(&camera.GetView());
    DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(45),
        Graphics::Instance().GetScreenWidth() / Graphics::Instance().GetScreenHeight(),
        nearCamera, farCamera);
    matrix = viewMat * projMat;

    //�r���[�v���W�F�N�V�����s��̋t�s��
    DirectX::XMMATRIX invMatrix = DirectX::XMMatrixInverse(nullptr, matrix);
    //�r���[�v���W�F�N�V�������̒��_�Z�o�p�ʒu�x�N�g��
    DirectX::XMVECTOR verts[8] =
    {
        // near plane corners
        { -1, -1, 0 },	// [0]:����
        {  1, -1, 0 },	// [1]:�E��
        {  1,  1, 0 },	// [2]:�E��
        { -1,  1 ,0 },	// [3]:����

        // far plane corners.
        { -1, -1, 1 },	// [4]:����
        {  1, -1, 1 },	// [5]:�E��
        {  1,  1, 1 },	// [6]:�E��
        { -1,  1, 1 } 	// [7]:����
    };

    // �r���[�v���W�F�N�V�����s��̋t�s���p���āA�e���_���Z�o����
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

    // ������i�t���X�^���j���\������U���ʂ��Z�o����
    // 0:������, 1:�E����, 2:������, 3:�㑤��, 4:������, 5:��O����
    // �S�Ă̖ʂ̖@���͓����������悤�ɐݒ肷�邱��

    // ������
    frustum[0].normal.x = matrix4X4._14 + matrix4X4._11;
    frustum[0].normal.y = matrix4X4._24 + matrix4X4._21;
    frustum[0].normal.z = matrix4X4._34 + matrix4X4._31;
    DirectX::XMStoreFloat3(&frustum[0].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[0].normal)));
    frustum[0].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[0].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // �E����
    frustum[1].normal.x = matrix4X4._14 - matrix4X4._11;
    frustum[1].normal.y = matrix4X4._24 - matrix4X4._21;
    frustum[1].normal.z = matrix4X4._34 - matrix4X4._31;
    DirectX::XMStoreFloat3(&frustum[1].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[1].normal)));
    frustum[1].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[1].normal), DirectX::XMLoadFloat3(&nearPoint[1])));

    // ������
    frustum[2].normal.x = matrix4X4._14 + matrix4X4._12;
    frustum[2].normal.y = matrix4X4._24 + matrix4X4._22;
    frustum[2].normal.z = matrix4X4._34 + matrix4X4._32;
    DirectX::XMStoreFloat3(&frustum[2].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[2].normal)));
    frustum[2].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[2].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // �㑤��
    frustum[3].normal.x = matrix4X4._14 - matrix4X4._12;
    frustum[3].normal.y = matrix4X4._24 - matrix4X4._22;
    frustum[3].normal.z = matrix4X4._34 - matrix4X4._32;
    DirectX::XMStoreFloat3(&frustum[3].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[3].normal)));
    frustum[3].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[3].normal), DirectX::XMLoadFloat3(&nearPoint[2])));

    // ������
    frustum[4].normal.x = -matrix4X4._14 - matrix4X4._13;
    frustum[4].normal.y = -matrix4X4._24 - matrix4X4._23;
    frustum[4].normal.z = -matrix4X4._34 - matrix4X4._33;
    DirectX::XMStoreFloat3(&frustum[4].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[4].normal)));
    frustum[4].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[4].normal), DirectX::XMLoadFloat3(&nearPoint[0])));

    // ��O����
    frustum[5].normal.x = -matrix4X4._14 + matrix4X4._13;
    frustum[5].normal.y = -matrix4X4._24 + matrix4X4._23;
    frustum[5].normal.z = -matrix4X4._34 + matrix4X4._33;
    DirectX::XMStoreFloat3(&frustum[5].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&frustum[5].normal)));
    frustum[5].distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum[5].normal), DirectX::XMLoadFloat3(&nearPoint[0])));
}

bool CameraController::frustumCulling(DirectX::XMFLOAT3 position, float radius)
{
    // ������i�t���X�^���j�Ƃ̌����E���O����
      // AABB��������̓����ɂ���			�FcollisionState = 0
      // AABB��������̋��E�ƌ�����Ă���	�FcollisionState = 1
      // AABB��������̊O���ɂ���			�FcollisionState = 2

      // ������������ɂ���Ƃ��Ă���
    collisionState = 0;

    DirectX::XMFLOAT3 enemyPosition = position;
    float enemyRadius = radius;
    for (int j = 0; j < 6; j++)
    {
        // �e���ʂ̖@���̐�����p����AABB�̂W���_�̒�����ŋߓ_�ƍŉ��_�����߂�
        DirectX::XMFLOAT3 NegaPos = enemyPosition;	// �ŋߓ_
        DirectX::XMFLOAT3 PosiPos = enemyPosition;	// �ŉ��_

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

        // �e���ʂƂ̓��ς��v�Z���A�����E���O������s��
        // �O���ƕ�����Ώ�����break���m�肳����
        // ������Ԃł���΁A�X�e�[�^�X��ύX���Ă��玟�̕��ʂƂ̃`�F�b�N�ɑ�����
        // �����ł���΁A���̂܂܎��̕��ʂƂ̃`�F�b�N�ɑ�����
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
        // �G�̕����Ƀ��C���΂�
        DirectX::XMFLOAT3 toEnemy = { 0, 0, 0 };
        DirectX::XMStoreFloat3(&toEnemy, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&Camera::Instance().GetEye())));

        // ���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { Camera::Instance().GetEye().x, Camera::Instance().GetEye().y + 0.5f, Camera::Instance().GetEye().z };
        DirectX::XMFLOAT3 end = { Camera::Instance().GetEye().x + toEnemy.x, Camera::Instance().GetEye().y + toEnemy.y + 0.5f, Camera::Instance().GetEye().z + toEnemy.z };

        return true;
    }

    return false;
}

DirectX::XMFLOAT3 CameraController::ResetCamera(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    const DirectX::XMFLOAT3 playerFront = player->GetFront();

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

void CameraController::GetTargetPerspective()
{
    // �v���[���[�����ԋ߂��G�l�~�[���Z�o����(�J���������ǂ����͖���)
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    EnemyManager& enemyManager = EnemyManager::Instance();

    auto enemyNumber = LockOnSwitching();
    DirectX::XMFLOAT3 i = player->GetPosition();
    DirectX::XMFLOAT3 length =
            Mathf::SubtractFloat3(enemyManager.GetEnemy(1)->GetPosition(), player->GetPosition());
        float square = sqrtf(powf(length.x, 2.0f) + powf(length.y, 2.0f) + powf(length.z, 2.0f));
        DirectX::XMFLOAT3 playerEnemyLength = {10, 10, 10};

        //nowTargetIndex = *enemyNumber;

    perspective = DirectX::XMFLOAT3(
        player->GetPosition().x - playerEnemyLength.x * playerRange,
        player->GetPosition().y - playerEnemyLength.y + lockOnPositionY,
        player->GetPosition().z - playerEnemyLength.z * playerRange);
}

DirectX::XMFLOAT3 CameraController::UpdateTransitionState(float elapsedTime)
{
    DirectX::XMFLOAT3 interpolation = {};
    DirectX::XMFLOAT3 start = GetPerspective();
    //interpolation = Mathf::LerpFloat3(start, perspective, 3.0f);
    //if (timerer > 1.0f)
    {
        state = CameraContorollerState::LockOnTargetState;
    }
    timerer += elapsedTime;
    return interpolation;
}

DirectX::XMFLOAT3 CameraController::GetPerspective()
{
    DirectX::XMFLOAT3 perspective;

    nowTargetIndex = -1;

    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR frontVec = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, frontVec);

    perspective.x = target.x - front.x * playerRange;
    perspective.y = target.y - front.y * playerRange;
    perspective.z = target.z - front.z * playerRange;

    return perspective;
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