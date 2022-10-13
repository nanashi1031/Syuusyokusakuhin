#include "StateBase.h"

// TODO 03_02 HierarchicalStateクラスの関数定義を行う
// 2層目ステートセット
void HierarchicalState::SetSubState(int newState)
{
	// TODO 03_02 前回のSetState関数を参考に記述しなさい
	subState = subStatePool.at(newState);
	subState->Enter();
}

// 2層目のステート切り替え
void HierarchicalState::ChangeSubState(int newState)
{
	// TODO 03_02 前回のChangeState関数を参考に記述しなさい
	// ステートの切り替え
	if (subState != nullptr)
	{
		//現在のステートのExit関数を実行
		subState->Exit();

		//新しいステートをセット
		subState = subStatePool.at(newState);

		//新しいステートのEnter関数を呼び出す。
		subState->Enter();
	}
}
// サブステート登録
void HierarchicalState::RegisterSubState(State* state)
{
	subStatePool.emplace_back(state);
}

// サブステートの番号取得
int HierarchicalState::GetSubStateIndex()
{
	int i = 0;
	for (State* state : subStatePool)
	{
		if (subState == state)return i;
		++i;
	}
	return i;
}
