#pragma once
#include <vector>
class Character;

class State
{
public:
	template<class T>
	State(T* t) :owner(t) {}
	virtual ~State() {}

	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;
protected:
	Character* owner;
};