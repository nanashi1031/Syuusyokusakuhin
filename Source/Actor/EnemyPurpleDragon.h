#pragma once
#include "Model.h"
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
		BattleIdle,
		Neglect,
		Howl,
		Pursuit,
		Walk,
		Run,
		TakeOff,
		Fly,
		Landing,
		Sleep,
		BiteAttack,
		ClawAttack,
		Damages,
		Die,
	};

	enum class Animation
	{
		IdleNormal,
		FryFound,
		AttackBiting,
		FryTakeOff,
		FryLanding,
		Howl,
		WingStrike,
		Flame,
		Defend,
		Wince,
		Sleep,
		WalkFound,
		RunFound,
		FryFrameAttack,
		FryGlide,
		BattleIdle,
		Die,
		FryFloat,
	};

	enum class PartGroup
	{
		Head,
		Body,
		Legs,
		Wings,
		Tail,
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

	// 部位の情報設定
	void PartsRegister();

	// セッターゲッター
	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// ターゲットポジション設定
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

protected:
	// ダメージを受けたときに呼ばれる
	void OnDamaged() override;

	// 死亡したときに呼ばれる
	void OnDead() override;

	// ダウン時に呼ばれる
	void OnDown() override;

private:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				searchRange = 10.0f;

	std::unique_ptr<AudioSource> SE_Attack;
};