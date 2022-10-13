#include "StateMachine.h"

StateMachine::~StateMachine()
{
	//�o�^�����X�e�[�g���폜����
	for (State* state : statePool)
	{
		delete state;
	}
	statePool.clear();
}
// �X�V����
void StateMachine::Update(float elapsedTime)
{
	currentState->Execute(elapsedTime);
}
// �X�e�[�g�Z�b�g
void StateMachine::SetState(int setState)
{
	currentState = statePool.at(setState);
	currentState->Enter();
}
// �X�e�[�g�ύX
void StateMachine::ChangeState(int newState)
{
	// TODO 02_02 �X�e�[�g�̐؂�ւ�
	// TODO 03_04 �X�e�[�g�̐؂�ւ��B
	// ���ݎ��s����Ă���X�e�[�g��Exit�֐������s
	// �X�e�[�g�̍X�V
	// �X�V���ꂽ�X�e�[�g��Enter�֐����s
	if (currentState != nullptr)
	{
		//���݂̃X�e�[�g��Exit�֐������s
		currentState->Exit();
		// ���ݎ��s����Ă���T�u�X�e�[�g��Exit�֐������s
		currentState->GetSubState()->Exit();

		//�V�����X�e�[�g���Z�b�g
		SetState(newState);

		//�V�����X�e�[�g��Enter�֐����Ăяo���B
		currentState->Enter();
	}
}

//�@�T�u�X�e�[�g�X�V
void StateMachine::ChangeSubState(int newState)
{
	// TODO 03_04 HierarchicalState�N���X��ChangeSubState���Ăяo��
	currentState->ChangeSubState(newState);
}

// �X�e�[�g�o�^
void StateMachine::RegisterState(HierarchicalState* state)
{
	// �e�X�e�[�g�o�^
	statePool.emplace_back(state);
}

// �T�u�X�e�[�g�o�^
void StateMachine::RegisterSubState(int index, State* subState)
{
	statePool.at(index)->RegisterSubState(subState);
}

// �X�e�[�g�ԍ��擾
int StateMachine::GetStateIndex()
{
	int i = 0;
	for (State* state : statePool)
	{
		if (state == currentState)
		{
			// i�ԍ��ڂ̃X�e�[�g�����^�[��
			return i;
		}
		++i;
	}
	// �X�e�[�g��������Ȃ�������
	return -1;
}