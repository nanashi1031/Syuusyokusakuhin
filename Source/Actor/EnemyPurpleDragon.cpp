#include "EnemyPurpleDragon.h"
#include "NodeBase.h"

EnemyPurpleDragon::EnemyPurpleDragon()
{
	model = new Model("Data/Model/Enemy/PurpleDragon/PurpleDragon.mdl");

	scale.x = scale.y = scale.z = 0.02f;

	radius = 0.5f;
	height = 0.5f;

	health = 10.0f;

	SetPlayAnimation(EnemyPurpleDragonAnimation::Sleep, true);
}

EnemyPurpleDragon::~EnemyPurpleDragon()
{

}

void EnemyPurpleDragon::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemyPurpleDragon::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemyPurpleDragon::DrawDebugPrimitive()
{

}