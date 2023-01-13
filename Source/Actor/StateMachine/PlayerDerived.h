#pragma once
#include "Player.h"
#include "StateMachine.h"

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
		bool nextAttackFlag = false;
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
		bool nextAttackFlag = false;
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
}