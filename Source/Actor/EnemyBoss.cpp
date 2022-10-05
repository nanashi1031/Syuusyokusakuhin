#include "EnemyBoss.h"

EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Enemy/PurpleDragon/PurpleDragon.mdl");

    scale.x = scale.y = scale.z = 0.02f;

    radius = 0.5f;
    height = 0.5f;

    health = 10.0f;

    model->PlayAnimation(6, true);
}

EnemyBoss::~EnemyBoss()
{
    delete model;
}

void EnemyBoss::Update(float elapsedTime)
{
    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemyBoss::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}