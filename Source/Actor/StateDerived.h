#pragma once

#include "StateBase.h"

class WanderState : public State
{
public:
    template<typename T>
    WanderState(T* character) :State(character) {};
    ~WanderState() {}
    void Enter() override;
    void Execute(float elapsedTime) override;
    void Exit() override;
};