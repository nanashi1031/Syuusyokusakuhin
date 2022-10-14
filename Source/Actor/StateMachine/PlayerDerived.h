#pragma once
#include "Player.h"
#include "StateMachine.h"

// アクションステート
class ActionState : public State
{
public:
	ActionState(Player* actor) : State(actor) {}
	~ActionState();

	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};

// 戦闘ステート
class BattleState : public HierarchicalState
{
public:
	BattleState(Player* actor) :HierarchicalState(actor) {}
	~BattleState();

	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};

// 徘徊ステート
class WanderState : public State
{
public:
	WanderState(Player* actor) :State(actor) {};
	~WanderState() {}

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

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
};

// 追跡ステート
class PursuitState : public State
{
public:
	PursuitState(Player* actor) :State(actor) {};
	~PursuitState() {}

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 攻撃ステート
class AttackState : public State
{
public:
	AttackState(Player* actor) :State(actor) {};
	~AttackState() {}

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};