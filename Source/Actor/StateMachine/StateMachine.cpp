#include "StateMachine.h"

StateMachine::~StateMachine()
{
	//登録したステートを削除する
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
// ステートセット
void StateMachine::SetState(int setState)
{
	currentState = statePool.at(setState);
	currentState->Enter();
}
// ステート変更
void StateMachine::ChangeState(int newState)
{
	// TODO 02_02 ステートの切り替え
	// TODO 03_04 ステートの切り替え。
	// 現在実行されているステートのExit関数を実行
	// ステートの更新
	// 更新されたステートのEnter関数実行
	if (currentState != nullptr)
	{
		//現在のステートのExit関数を実行
		currentState->Exit();
		// 現在実行されているサブステートのExit関数を実行
		currentState->GetSubState()->Exit();

		//新しいステートをセット
		SetState(newState);

		//新しいステートのEnter関数を呼び出す。
		currentState->Enter();
	}
}

//　サブステート更新
void StateMachine::ChangeSubState(int newState)
{
	// TODO 03_04 HierarchicalStateクラスのChangeSubStateを呼び出す
	currentState->ChangeSubState(newState);
}

// ステート登録
void StateMachine::RegisterState(HierarchicalState* state)
{
	// 親ステート登録
	statePool.emplace_back(state);
}

// サブステート登録
void StateMachine::RegisterSubState(int index, State* subState)
{
	statePool.at(index)->RegisterSubState(subState);
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