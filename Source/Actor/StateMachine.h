#pragma once

#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
    StateMachine() {}
    ~StateMachine();

    void Update(float elapsedTime);

    void SetState(int newState);
    void ChangeState(int newState);
    // �X�e�[�g�o�^
    void RegisterState(State* state);

    int GetStateindex();

private:
    State* currentState = nullptr;
    std::vector<State*> statePool;
};