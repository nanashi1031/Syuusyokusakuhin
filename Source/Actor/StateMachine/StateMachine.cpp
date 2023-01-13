#include "StateMachine.h"

StateMachine::~StateMachine()
{
	for (State* state : statePool)
	{
		delete state;
	}
	statePool.clear();
}
// 更新処理
void StateMachine::Update(float elapsedTime)
{
	currentState->Execute(elapsedTime);
}

// ステート登録
void StateMachine::RegisterState(State* state)
{
	// 親ステート登録
	statePool.emplace_back(state);
}

// ステート番号取得
int StateMachine::GetStateIndex()
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
	// ステートが見つからなかった時
	return -1;
}