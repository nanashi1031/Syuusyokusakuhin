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
	void ChangeState(T newState)
	{
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

	// ステート登録
	void RegisterState(HierarchicalState* state);

	// 現在のステート番号取得
	int GetStateIndex();

	// ２層目ステート変更
	template<typename T>
	void ChangeSubState(T newState)
	{
		int state = static_cast<int>(newState);
		currentState->ChangeSubState(state);
	}

	// ２層目ステート登録
	template<typename T>
	void RegisterSubState(T tIndex, State* subState)
	{
		int index = static_cast<int>(tIndex);
		statePool.at(index)->RegisterSubState(subState);
	}

	// ステート取得
	HierarchicalState* GetState() { return currentState; }

private:
	// 現在のステート
	HierarchicalState* currentState = nullptr;

	// 各ステートを保持する配列
	std::vector<HierarchicalState*> statePool;
};