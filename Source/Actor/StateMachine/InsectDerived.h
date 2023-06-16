#pragma once
#include "Insect.h"
#include "StateMachine.h"


namespace InsectState
{
	// 待機ステート
	class IdleState : public State
	{
	public:
		IdleState(Insect* actor) :State(actor) {};
		~IdleState() {}

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
		PursuitState(Insect* actor) : State(actor) {};
		~PursuitState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		std::unique_ptr<AudioSource> SE_Heal;
		std::unique_ptr<AudioSource> SE_ExtractGet;
	};

	// 飛翔ステート
	class FlyingState : public State
	{
	public:
		FlyingState(Insect* actor) : State(actor) {}
		~FlyingState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	private:
		float timer = 0.0f;
		std::unique_ptr<AudioSource> SE_Whistle;
		std::unique_ptr<AudioSource> SE_Hit;
	};

	// 帰還ステート
	class ReturnState : public State
	{
	public:
		ReturnState(Insect* actor) : State(actor) {}
		~ReturnState() {}

		// ステートに入った時のメソッド
		void Enter()override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime)override;
		// ステートから出ていくときのメソッド
		void Exit()override;
	};
}