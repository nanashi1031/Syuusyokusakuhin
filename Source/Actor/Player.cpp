#include <imgui.h>
#include "Input\Input.h"
#include "Player.h"

Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");

    scale.x = scale.y = scale.z = k_size;
}

Player::~Player()
{
    delete model;
}

void Player::Update(float elapsedTime)
{
    Move();

    UpdateTransform();

    model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 120), ImGuiWindowFlags_NoTitleBar);
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::DragFloat3("Postion", &position.x, 0.1f);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
            ImGui::DragFloat3("Scale", &scale.x, 0.0005, 0, 1000);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void Player::Move()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // �X�e�B�b�N�̈ړ��l���擾
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // ���X�e�B�b�N�̓��͏������Ƃ�XZ���ʂւ̈ړ�����
    position.x += ax * moveSpeed;
    position.z += ay * moveSpeed;


}