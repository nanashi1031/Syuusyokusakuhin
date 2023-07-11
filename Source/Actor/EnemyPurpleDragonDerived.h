#pragma once
#include "EnemyPurpleDragon.h"

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
		enum class IdleStateChange
		{
			Pursuit,
			fly,
		};

		float stateTimer = 0.0f;
	};

	// �퓬�ҋ@�X�e�[�g
	class BattleIdleState : public State
	{
	public:
		BattleIdleState(EnemyPurpleDragon* actor) :State(actor) {};
		~BattleIdleState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;

	private:
		enum class BattleIdleStateChange
		{
			Pursuit,
			fly,
		};

		float stateTimer = 3.0f;
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

	// ���K�X�e�[�g
	class HowlState : public State
	{
	public:
		HowlState(EnemyPurpleDragon* actor) :State(actor) {};
		~HowlState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float seTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_Howl1;
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
		enum class PursuitStateChange
		{
			BiteAttack,
			ClawAttack,
		};

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

	// �����X�e�[�g
	class TakeOffState : public State
	{
	public:
		TakeOffState(EnemyPurpleDragon* actor) :State(actor) {};
		~TakeOffState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Flapping;
	};

	// ��s�X�e�[�g
	class FlyState : public State
	{
	public:
		FlyState(EnemyPurpleDragon* actor) :State(actor) {};
		~FlyState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

	// �����X�e�[�g
	class LandingState : public State
	{
	public:
		LandingState(EnemyPurpleDragon* actor) :State(actor) {};
		~LandingState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float seTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Flapping;
	};

	// �����X�e�[�g
	class SleepState : public State
	{
	public:
		SleepState(EnemyPurpleDragon* actor) :State(actor) {};
		~SleepState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	};

	// ���݂��U���X�e�[�g
	class BiteAttackState : public State
	{
	public:
		BiteAttackState(EnemyPurpleDragon* actor) :State(actor) {};
		~BiteAttackState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_Bite;
	};

	// �܍U���X�e�[�g
	class ClawAttackState : public State
	{
	public:
		ClawAttackState(EnemyPurpleDragon* actor) :State(actor) {};
		~ClawAttackState() {}

		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter()override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime)override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit()override;
	private:
		float stateTimer = 0.0f;
		std::unique_ptr<AudioSource> SE_Claw;
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
	private:
		float seTimer = 0.0f;
		bool seFlag = false;
		std::unique_ptr<AudioSource> SE_FallDown;
	};
}