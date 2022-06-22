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
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}