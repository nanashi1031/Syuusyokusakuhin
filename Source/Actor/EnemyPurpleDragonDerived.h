#pragma once
#include "EnemyPurpleDragon.h"
#include "StateMachine.h"

namespace EnemyPurpleDragonState
{
	// 待機ステート
	class IdleState : public State
	{
	public:
		IdleState(EnemyPurpleDragon* actor) :State(actor) {};
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
		NeglectState(EnemyPurpleDragon* actor) :State(actor) {};
		~NeglectState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};

	// 追跡ステート
	class PursuitState : public State
	{
	public:
		PursuitState(EnemyPurpleDragon* actor) :State(actor) {};
		~PursuitState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
	};

	// 歩きステート
	class WalkState : public State
	{
	public:
		WalkState(EnemyPurpleDragon* actor) :State(actor) {};
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
		RunState(EnemyPurpleDragon* actor) :State(actor) {};
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
		AttackCombo1State(EnemyPurpleDragon* actor) :State(actor) {};
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


	// ダメージステート
	class DamagesState : public State
	{
	public:
		DamagesState(EnemyPurpleDragon* actor) :State(actor) {};
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
		DieState(EnemyPurpleDragon* actor) :State(actor) {};
		~DieState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};
}