#include "StateBase.h"

void HierarchicalState::SetSubState(int newState)
{
	subState = subStatePool.at(newState);
	subState->Enter();
}

void HierarchicalState::ChangeSubState(int newState)
{
	if (subState != nullptr)
	{
		subState->Exit();

		subState = subStatePool.at(newState);

		subState->Enter();
	}
}

// サブステート登録
void HierarchicalState::RegisterSubState(State* state)
{
	subStatePool.emplace_back(state);
}

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
