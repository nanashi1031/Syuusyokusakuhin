#include "CameraController.h"
#include "Camera.h"
#include "Input\Input.h"
#include <imgui.h>

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

    // �J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����̎��_�ƒ����_��ݒ�
    DirectX::XMFLOAT3 up = { 0, 1, 0 };
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(up));

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
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 120), ImGuiWindowFlags_NoTitleBar);
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::LabelText(" ", "target (%.1f , %.1f , %.1f)", target.x, target.y, target.z);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // ��]���x
            ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 5.0f);
            // �����W
            ImGui::SliderFloat("range", &range, 0.001f, 50.0f);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}