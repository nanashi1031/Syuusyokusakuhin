#include "EnemyBoss.h"

EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;
    height = 0.5f;

    health = 10.0f;
}

EnemyBoss::~EnemyBoss()
{
    delete model;
}

void EnemyBoss::Update(float elapsedTime)
{
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemyBoss::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}