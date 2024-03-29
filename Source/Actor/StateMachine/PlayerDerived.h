#pragma once
#include "Player.h"
#include "StateMachine.h"
#include "Effect.h"

namespace PlayerState
{
	// 待機ステート
	class IdleState : public State
	{
	public:
		IdleState(Player* actor) :State(actor) {};
		~IdleState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;

	private:
		float stateTimer = 0.0f;
	};

	// 放置ステート
	class NeglectState : public State
	{
	public:
		NeglectState(Player* actor) :State(actor) {};
		~NeglectState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		std::unique_ptr<AudioSource> NeglectVoice0;
		std::unique_ptr<AudioSource> NeglectVoice1;
		std::unique_ptr<AudioSource> NeglectVoice2;
	};

	// 歩きステート
	class WalkState : public State
	{
	public:
		WalkState(Player* actor) :State(actor) {};
		~WalkState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};

	// 走りステート
	class RunState : public State
	{
	public:
		RunState(Player* actor) :State(actor) {};
		~RunState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};

	// コンボ攻撃1ステート
	class AttackCombo1State : public State
	{
	public:
		AttackCombo1State(Player* actor) :State(actor) {};
		~AttackCombo1State() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		float hitStopTimer = 0.0f;
		bool nextAttackFlag = false;
		std::unique_ptr<AudioSource> SE_Attack1;
		Effect* hitEffect1 = nullptr;
	};
		// コンボ攻撃2ステート
	class AttackCombo2State : public State
	{
	public:
		AttackCombo2State(Player* actor) :State(actor) {};
		~AttackCombo2State() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;

	private:
		float stateTimer = 0.0f;
		float hitStopTimer = 0.0f;
		bool nextAttackFlag = false;
		std::unique_ptr<AudioSource> SE_Attack2;
		Effect* hitEffect2 = nullptr;
	};
	// コンボ攻撃3ステート
	class AttackCombo3State : public State
	{
	public:
		AttackCombo3State(Player* actor) :State(actor) {};
		~AttackCombo3State() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float hitStopTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Attack3;
		Effect* hitEffect3 = nullptr;
	};

	// ダッシュ攻撃ステート
	class AttackDashuState : public State
	{
	public:
		AttackDashuState(Player* actor) :State(actor) {};
		~AttackDashuState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;

	private:
		float stateTimer = 0.0f;
		bool nextAttackFlag = false;
	};

	// 回避ステート
	class AvoiDanceState : public State
	{
	public:
		AvoiDanceState(Player* actor) :State(actor) {};
		~AvoiDanceState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
	};

	// ダメージステート
	class DamagesState : public State
	{
	public:
		DamagesState(Player* actor) :State(actor) {};
		~DamagesState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		std::unique_ptr<AudioSource> DamageVoice;
	};

	//死亡ステート
	class DieState : public State
	{
	public:
		DieState(Player* actor) :State(actor) {};
		~DieState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};

	class Versatility
	{
	public:
		// スティック入力値から方向ベクトルを求める
		static DirectX::XMFLOAT3 Direction();

		// 方向ベクトルの向きへ向く
		static void Rotate(DirectX::XMFLOAT3 vec);
	};
}