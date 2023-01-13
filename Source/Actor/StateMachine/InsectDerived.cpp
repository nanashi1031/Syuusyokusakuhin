#include "Input\Input.h"
#include "InsectDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "Camera.h"

void InsectState::IdleState::Enter()
{

}

void InsectState::IdleState::Execute(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());
    //owner->SetPosition(player->GetPosition());

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E)
        owner->GetStateMachine()->ChangeState(Insect::State::Flying);

    if (gamePad.GetButton() & GamePad::KEY_R)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);
}

void InsectState::IdleState::Exit()
{

}

void InsectState::FlyingState::Enter()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    Enemy* enemy = enemyManager.GetEnemy(0);
    //owner->SetTargetPosition(enemy->GetPosition());

    DirectX::XMFLOAT3 direction = Mathf::MultiplyFloat3Float(Camera::Instance().GetCameraDirection(), 1.0f);
    owner->SetTargetPosition(Mathf::MultiplyFloat3(enemy->GetPosition(), direction));

    owner->SetMoveVecX(Camera::Instance().GetCameraDirection().x * 1000.0f);
    owner->SetMoveVecZ(Camera::Instance().GetCameraDirection().z * 1000.0f);

    owner->SetTargetPosition(Camera::Instance().GetFocus());

    timer = 0.0f;
}

void InsectState::FlyingState::Execute(float elapsedTime)
{
    //owner->MoveToTarget(elapsedTime, owner->GetMoveSpeed());

    // ‰ñ“](‰¼)
    owner->SetAngle({ owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z + 0.1f });
    //owner->Ratate({ 0.0f, 0.0f, 1.0f }, 1.0f);

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_R)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);

    if (timer > 5.0f)
    {
        owner->GetStateMachine()->ChangeState(Insect::State::Idle);
    }
    timer += 0.1f;
}

void InsectState::FlyingState::Exit()
{

}

void InsectState::ReturnState::Enter()
{

}

void InsectState::ReturnState::Execute(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());
    owner->SetTargetPosition(player->GetPosition());

    owner->MoveToTarget(elapsedTime, owner->GetMoveSpeed());

    // ‰ñ“](‰¼)
    owner->SetAngle({ owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z + 0.01f });
    //owner->Ratate({ 0.0f, 0.0f, 1.0f }, 1.0f);

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E)
        owner->GetStateMachine()->ChangeState(Insect::State::Flying);
}

void InsectState::ReturnState::Exit()
{

}