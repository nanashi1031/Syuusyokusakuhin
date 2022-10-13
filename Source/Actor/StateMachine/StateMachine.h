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
	void SetState(int setstate);
	// ステート変更
	void ChangeState(int newState);
	// TODO 03_04 第１階層のステート登録にあたり引数の型を変更
	// ステート登録
	void RegisterState(HierarchicalState* state);
	// 現在のステート番号取得
	int GetStateIndex();
	// TODO 03_04 新規で第２階層のステート関連を登録
	// ２層目ステート変更
	void ChangeSubState(int newState);
	// ２層目ステート登録
	void RegisterSubState(int index, State* subState);
	// ステート取得
	HierarchicalState* GetState() { return currentState; }
	// TODO 03_04 第１階層のステート登録にあたり型を変更

private:
	// TODO 03_04 第１階層のステート登録にあたり型を変更
	// 現在のステート
	HierarchicalState* currentState = nullptr;
	// TODO 03_04 ステートを保持するvectorの保存型を変更
	// 各ステートを保持する配列
	std::vector<HierarchicalState*> statePool;
};