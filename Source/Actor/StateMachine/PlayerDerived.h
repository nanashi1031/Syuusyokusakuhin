#pragma once
#include "Player.h"
#include "StateMachine.h"

// �A�N�V�����X�e�[�g
class ActionState : public HierarchicalState
{
public:
	ActionState(Player* actor) : HierarchicalState(actor) {}
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

// �ҋ@�X�e�[�g
class IdleState : public State
{
public:
	IdleState(Player* actor) :State(actor) {};
	~IdleState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// ���u�X�e�[�g
class NeglectState : public State
{
public:
	NeglectState(Player* actor) :State(actor) {};
	~NeglectState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �����X�e�[�g
class WalkState : public State
{
public:
	WalkState(Player* actor) :State(actor) {};
	~WalkState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// ����X�e�[�g
class RunState : public State
{
public:
	RunState(Player* actor) :State(actor) {};
	~RunState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �R���{�U��1�X�e�[�g
class AttackCombo1State : public State
{
public:
	AttackCombo1State(Player* actor) :State(actor) {};
	~AttackCombo1State() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �R���{�U��2�X�e�[�g
class AttackCombo2State : public State
{
public:
	AttackCombo2State(Player* actor) :State(actor) {};
	~AttackCombo2State() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �R���{�U��3�X�e�[�g
class AttackCombo3State : public State
{
public:
	AttackCombo3State(Player* actor) :State(actor) {};
	~AttackCombo3State() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �_�b�V���U���X�e�[�g
class AttackDashuState : public State
{
public:
	AttackDashuState(Player* actor) :State(actor) {};
	~AttackDashuState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// ����X�e�[�g
class AvoiDanceState : public State
{
public:
	AvoiDanceState(Player* actor) :State(actor) {};
	~AvoiDanceState() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};