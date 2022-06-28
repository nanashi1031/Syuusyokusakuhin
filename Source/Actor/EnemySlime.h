#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "StateMachine.h"

// エネミーアニメーション
enum class EnemyAnimation
{
	IdleNormal,
	IdleBattle,
	Attack01,
	Attack02,
	WalkFWD,
	WalkBWD,
	WalkLeft,
	WalkRight,
	RunFWD,
	SenseSomethingST,
	SenseSomethingRPT,
	Taunt,
	Victory,
	GetHit,
	Dizzy,
	Die,
};

class EnemySlime :public Enemy
{
public:
	EnemySlime();
	~EnemySlime()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// デバッグエネミー情報表示
	void DrawDebugGUI();
	// ターゲット位置をランダム設定
	void SetRandomTargetPosition();

	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

	// プレイヤー索敵
	bool SearchPlayer();

	// ターゲットポジション設定
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// ターゲットポジション取得
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// ポジション取得
	DirectX::XMFLOAT3 GetPosition() { return position; }

	// ステートタイマー設定
	void SetStateTimer(float timer) {
		stateTimer = timer;
	}
	// ステートタイマー取得
	float GetStateTimer() { return stateTimer; }

	// 攻撃範囲取得
	float GetAttackRange() { return attackRange; }

	//ステートマシン取得
	StateMachine* GetStateMachine() { return stateMachine; }

	//モデル取得
	Model* GetModel() { return model; }
protected:
	void OnDead();

public:
	// TODO 01 必要なステートをenum classで準備しなさい。
	// （Wanderのみ記述済み）
	// 各階層ごとに分割
	enum class State
	{
		Search,
		Battle,
	};

	enum class Search
	{
		Wander,
		Idle,
	};

	enum class Battle
	{
		Pursuit,
		Attack,
	};

private:
	Model* model = nullptr;
	State				state = State::Search;
	StateMachine* stateMachine = nullptr;
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				moveSpeed = 3.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	float				searchRange = 5.0f;
	float				attackRange = 1.5f;
};