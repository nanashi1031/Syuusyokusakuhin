#pragma once
#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
	// コンストラクタ
	StateMachine() {}
	// デストラクタ
	~StateMachine();
	// 更新処理
	void Update(float elapsedTime);

	// ステートセット
	template<typename T>
	void SetState(T tSetstate)
	{
		int setState = static_cast<int>(tSetstate);
		currentState = statePool.at(setState);
		currentState->Enter();
	}

	// ステート変更
	template<typename T>
	void ChangeState(T tNewState)
	{
		if (currentState != nullptr)
		{
			//現在のステートのExit関数を実行
			currentState->Exit();

			//新しいステートをセット
			SetState(tNewState);

			//新しいステートのEnter関数を呼び出す。
			currentState->Enter();
		}
	}

	// ステート登録
	void RegisterState(State* state);

	// 現在のステート番号取得
	int GetStateIndex();

private:
	// 現在のステート
	State* currentState = nullptr;
	// 各ステートを保持する配列
	std::vector<State*> statePool;
};