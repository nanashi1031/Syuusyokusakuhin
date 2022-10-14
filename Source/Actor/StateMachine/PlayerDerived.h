#pragma once
#include "Player.h"
#include "StateMachine.h"

// �A�N�V�����X�e�[�g
class ActionState : public State
{
public:
	ActionState(Player* actor) : State(actor) {}
	~ActionState();

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

// �퓬�X�e�[�g
class BattleState : public HierarchicalState
{
public:
	BattleState(Player* actor) :HierarchicalState(actor) {}
	~BattleState();

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

// �p�j�X�e�[�g
class WanderState : public State
{
public:
	WanderState(Player* actor) :State(actor) {};
	~WanderState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g
class IdleState : public State
{
public:
	IdleState(Player* actor) :State(actor) {};
	~IdleState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ǐՃX�e�[�g
class PursuitState : public State
{
public:
	PursuitState(Player* actor) :State(actor) {};
	~PursuitState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �U���X�e�[�g
class AttackState : public State
{
public:
	AttackState(Player* actor) :State(actor) {};
	~AttackState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};