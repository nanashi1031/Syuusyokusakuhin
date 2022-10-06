#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

class EnemyPurpleDragon : public Enemy
{
public:
    EnemyPurpleDragon();
    ~EnemyPurpleDragon() override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;
};