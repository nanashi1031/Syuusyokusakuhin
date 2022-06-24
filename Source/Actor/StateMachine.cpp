#include "StateMachine.h"

StateMachine::~StateMachine()
{
    for (State* state : statePool)
    {
        delete state;
    }

    statePool.clear();
}

void StateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

void StateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);
    currentState->Enter();
}

void StateMachine::ChangeState(int newState)
{
	if (currentState != nullptr)
	{
		//���݂̃X�e�[�g��Exit�֐������s
		currentState->Exit();

		//�V�����X�e�[�g���Z�b�g
		SetState(newState);

		//�V�����X�e�[�g��Enter�֐����Ăяo���B
		currentState->Enter();
	}
}

// �X�e�[�g�o�^
void StateMachine::RegisterState(State* state)
{
	// �e�X�e�[�g�o�^
	statePool.emplace_back(state);
}

int StateMachine::GetStateindex()
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
	return -1;
}