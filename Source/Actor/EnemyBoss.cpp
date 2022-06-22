#include "EnemyBoss.h"

EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    scale.x = scale.y = scale.z = 0.01f;
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