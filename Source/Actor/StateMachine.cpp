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
		//現在のステートのExit関数を実行
		currentState->Exit();

		//新しいステートをセット
		SetState(newState);

		//新しいステートのEnter関数を呼び出す。
		currentState->Enter();
	}
}

// ステート登録
void StateMachine::RegisterState(State* state)
{
	// 親ステート登録
	statePool.emplace_back(state);
}

int StateMachine::GetStateindex()
{
	int i = 0;
	for (State* state : statePool)
	{
		if (state == currentState)
		{
			// i番号目のステートをリターン
			return i;
		}
		++i;
	}
	return -1;
}