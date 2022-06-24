#pragma once
#include <vector>
#include "Character.h"

class State
{
public:
	template<typename T>
	State(T* character) :owner(character) {}
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