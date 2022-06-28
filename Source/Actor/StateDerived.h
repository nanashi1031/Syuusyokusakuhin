#pragma once
#include "EnemySlime.h"

class SearchState : public HierarchicalState
{
public:
	SearchState(EnemySlime* enemy) :HierarchicalState(enemy) {}
	~SearchState();

	void Enter();
	void Execute(float elapsedTime);
	void Exit();
};

class BattleState : public HierarchicalState
{
public:
	BattleState(EnemySlime* enemy) :HierarchicalState(enemy) {}
	~BattleState();

	void Enter();
	void Execute(float elapsedTime);
	void Exit();
};

class WanderState : public State
{
public:
	WanderState(EnemySlime* enemy) :State(enemy) {};
	~WanderState() {}

	void Enter()override;
	void Execute(float elapsedTime)override;
	void Exit()override;
};

class IdleState : public State
{
public:
	IdleState(EnemySlime* enemy) :State(enemy) {};
	~IdleState() {}

	void Enter()override;
	void Execute(float elapsedTime)override;
	void Exit()override;
};

class PursuitState : public State
{
public:
	PursuitState(EnemySlime* enemy) :State(enemy) {};
	~PursuitState() {}

	void Enter()override;
	void Execute(float elapsedTime)override;
	void Exit()override;
};

class AttackState : public State
{
public:
	AttackState(EnemySlime* enemy) :State(enemy) {};
	~AttackState() {}

	void Enter()override;
	void Execute(float elapsedTime)override;
	void Exit()override;
};