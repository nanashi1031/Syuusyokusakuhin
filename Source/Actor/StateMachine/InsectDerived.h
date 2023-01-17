#pragma once
#include "Insect.h"
#include "StateMachine.h"


namespace InsectState
{
	// �ҋ@�X�e�[�g
	class IdleState : public State
	{
	public:
		IdleState(Insect* actor) :State(actor) {};
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
		PursuitState(Insect* actor) : State(actor) {};
		~PursuitState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	};

	// ���ăX�e�[�g
	class FlyingState : public State
	{
	public:
		FlyingState(Insect* actor) : State(actor) {}
		~FlyingState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float timer = 0.0f;
	};

	// �A�҃X�e�[�g
	class ReturnState : public State
	{
	public:
		ReturnState(Insect* actor) : State(actor) {}
		~ReturnState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	};
}