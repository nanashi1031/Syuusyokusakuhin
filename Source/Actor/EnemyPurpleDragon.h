#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "Audio.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;

class EnemyPurpleDragon : public Enemy
{
public:
	enum class State
	{
		Idle,
		Neglect,
		Pursuit,
		Walk,
		Run,
		AttackCombo,
		Damages,
		Die,
	};

	enum class EnemyPurpleDragonAnimation
	{
		IdleNormal,
		FryFound,
		AttackBiting,
		FryTakeOf,
		FryLanding,
		Scream,
		WingStrike,
		Flame,
		Defend,
		Wince,
		Sleep,
		WalkFound,
		RunFound,
		FryFrameAttack,
		FryGlide,
		IdelBattle,
		Die,
		FryFloat,
	};

public:
    EnemyPurpleDragon();
    ~EnemyPurpleDragon() override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(RenderContext rc, ModelShader* shader)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	void DrawDebugGUI() override;

	void OnDamaged() override;

	void PartsRegister();

	// セッターゲッター
	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// ターゲットポジション設定
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

private:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				searchRange = 10.0f;

	std::unique_ptr<AudioSource> SE_Attack;
};