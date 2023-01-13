#include "StateMachine.h"

StateMachine::~StateMachine()
{
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

// �X�e�[�g�o�^
void StateMachine::RegisterState(State* state)
{
	// �e�X�e�[�g�o�^
	statePool.emplace_back(state);
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