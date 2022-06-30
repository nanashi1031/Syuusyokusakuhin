#pragma once
#include <vector>
class EnemySlime;

class State
{
public:

	template<class T>
	//EnemySlimeにしか対応してないのでテンプレート化する
	State(T* t) :owner(t) {}
	virtual ~State() {}

	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;
protected:
	EnemySlime* owner;
};

class HierarchicalState : public State
{
public :
	template<class T>
	HierarchicalState(T* t) : State((t)) {}
	virtual ~HierarchicalState() {}

	//ステートに入った時のメソッド
	virtual void Enter() = 0;
	//ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	//ステートから出ていくときのメソッド
	virtual void Exit() = 0;
	//サブステート登録
	virtual void SetSubState(int newState);
	//サブステート変更
	virtual void ChangeSubState(int newState);

	virtual void RegisterSubState(State* state);

	virtual State* GetSubState() { return subState; }

	virtual int GetSubStateIndex();

protected:
	std::vector<State*> subStatePool;
	State* subState = nullptr;
};