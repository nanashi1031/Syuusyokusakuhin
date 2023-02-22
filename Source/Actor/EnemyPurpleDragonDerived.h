#pragma once
#include "EnemyPurpleDragon.h"
#include "StateMachine.h"

namespace EnemyPurpleDragonState
{
	// �ҋ@�X�e�[�g
	class IdleState : public State
	{
	public:
		IdleState(EnemyPurpleDragon* actor) :State(actor) {};
		~IdleState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;

	private:
		float stateTimer = 0.0f;
	};

	// ���u�X�e�[�g
	class NeglectState : public State
	{
	public:
		NeglectState(EnemyPurpleDragon* actor) :State(actor) {};
		~NeglectState() {}

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
		PursuitState(EnemyPurpleDragon* actor) :State(actor) {};
		~PursuitState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float stateTimer = 0.0f;
	};

	// �����X�e�[�g
	class WalkState : public State
	{
	public:
		WalkState(EnemyPurpleDragon* actor) :State(actor) {};
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
		RunState(EnemyPurpleDragon* actor) :State(actor) {};
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
		AttackCombo1State(EnemyPurpleDragon* actor) :State(actor) {};
		~AttackCombo1State() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		bool nextAttackFlag = false;
	};


	// �_���[�W�X�e�[�g
	class DamagesState : public State
	{
	public:
		DamagesState(EnemyPurpleDragon* actor) :State(actor) {};
		~DamagesState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	};

	//���S�X�e�[�g
	class DieState : public State
	{
	public:
		DieState(EnemyPurpleDragon* actor) :State(actor) {};
		~DieState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	};
}