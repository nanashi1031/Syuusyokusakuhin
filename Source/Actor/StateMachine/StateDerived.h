#pragma once
#include "EnemySlime.h"

// TODO 03_03 HierarchicalState���p�������e�X�e�[�g�ł���
// BattleState�N���X���쐬���Ȃ����B
// �܂��e�֐���`��StateDerived.cpp�ɒǉ����Ȃ����B
// �������̗�Ƃ���SearchState�N���X�͋L�q���Ă����܂��B
class SearchState : public HierarchicalState
{
public:
	// �R���X�g���N�^
	SearchState(EnemySlime* enemy) :HierarchicalState(enemy) {}
	~SearchState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

class BattleState : public HierarchicalState
{
public:
	// �R���X�g���N�^
	BattleState(EnemySlime* enemy) :HierarchicalState(enemy) {}
	~BattleState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
	// �R���X�g���N�^
	WanderState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class PursuitState : public State
{
public:
	// �R���X�g���N�^
	PursuitState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~PursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public State
{
public:
	// �R���X�g���N�^
	AttackState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};