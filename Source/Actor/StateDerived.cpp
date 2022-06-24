#include "StateDerived.h"

void WanderState::Enter()
{
    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
}

void WanderState::Execute(float elapsedTime)
{

}

void WanderState::Exit()
{

}