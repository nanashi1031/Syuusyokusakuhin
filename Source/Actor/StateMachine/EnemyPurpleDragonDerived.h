#pragma once
#include "EnemyPurpleDragon.h"

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
		enum class IdleStateChange
		{
			Pursuit,
			fly,
		};

		float stateTimer = 0.0f;
	};

	// 戦闘待機ステート
	class BattleIdleState : public State
	{
	public:
		BattleIdleState(EnemyPurpleDragon* actor) :State(actor) {};
		~BattleIdleState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;

	private:
		enum class BattleIdleStateChange
		{
			Pursuit,
			fly,
		};

		float stateTimer = 3.0f;
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

	// 咆哮ステート
	class HowlState : public State
	{
	public:
		HowlState(EnemyPurpleDragon* actor) :State(actor) {};
		~HowlState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float seTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_Howl1;
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
		enum class PursuitStateChange
		{
			BiteAttack,
			ClawAttack,
		};

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

	// 離陸ステート
	class TakeOffState : public State
	{
	public:
		TakeOffState(EnemyPurpleDragon* actor) :State(actor) {};
		~TakeOffState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Flapping;
	};

	// 飛行ステート
	class FlyState : public State
	{
	public:
		FlyState(EnemyPurpleDragon* actor) :State(actor) {};
		~FlyState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		enum class FlyStateChange
		{
			Landing,
			TakeOff,
		};

		float stateTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Flapping;
	};

	// 着陸ステート
	class LandingState : public State
	{
	public:
		LandingState(EnemyPurpleDragon* actor) :State(actor) {};
		~LandingState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float seTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Flapping;
	};

	// 睡眠ステート
	class SleepState : public State
	{
	public:
		SleepState(EnemyPurpleDragon* actor) :State(actor) {};
		~SleepState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};

	// 噛みつき攻撃ステート
	class BiteAttackState : public State
	{
	public:
		BiteAttackState(EnemyPurpleDragon* actor) :State(actor) {};
		~BiteAttackState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_Bite;
	};

	// 爪攻撃ステート
	class ClawAttackState : public State
	{
	public:
		ClawAttackState(EnemyPurpleDragon* actor) :State(actor) {};
		~ClawAttackState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Claw;
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
	private:
		float seTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_FallDown;
	};
}