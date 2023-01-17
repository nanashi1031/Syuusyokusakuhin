#include "Input\Input.h"
#include "InsectDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "Camera.h"
#include "CameraController.h"

void InsectState::IdleState::Enter()
{

}

void InsectState::IdleState::Execute(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

    DirectX::XMFLOAT3 length =
        Mathf::SubtractFloat3(owner->GetPosition(), player->GetPosition());
    length = Mathf::MakePlusFloat3(length);
    if (length.x < 5.0f && length.z < 5.0f)
    {
        owner->GetStateMachine()->ChangeState(Insect::State::Pursuit);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E)
        owner->GetStateMachine()->ChangeState(Insect::State::Flying);

    if (gamePad.GetButton() & GamePad::KEY_R)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);
}

void InsectState::IdleState::Exit()
{

}

void InsectState::PursuitState::Enter()
{

}

void InsectState::PursuitState::Execute(float elapsedTime)
{
    Player* player  = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex());
    owner->SetPosition(player->GetPosition());

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E)
        owner->GetStateMachine()->ChangeState(Insect::State::Flying);

    owner->SetPosition({ owner->GetPosition().x, 3.0f, owner->GetPosition().z });
}

void InsectState::PursuitState::Exit()
{

}

void InsectState::FlyingState::Enter()
{
    CameraController& cameraController = CameraController::Instance();
    EnemyManager& enemyManager = EnemyManager::Instance();
    if (cameraController.GetLockOnFlag())
    {
        Enemy* enemy = enemyManager.GetEnemy(0);
        // ƒ^[ƒQƒbƒg‰ÓŠ‚Ö”ò‚Ô‚æ‚¤‚É
        Model::Node* node = enemy->GetModel()->FindNode(
            enemy->GetParts()[CameraController::Instance().GetTagetIndex()].name);
        DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(
            enemy->GetNodePosition(node), owner->GetPosition());
        owner->SetMoveVecX(lenght.x * 1000.0f);
        owner->SetMoveVecZ(lenght.z * 1000.0f);
        owner->SetVerocity(lenght);
    }
    else
    {
        Enemy* enemy = enemyManager.GetEnemy(0);

        DirectX::XMFLOAT3 direction = Mathf::MultiplyFloat3Float(Camera::Instance().GetCameraDirection(), 1.0f);
        owner->SetTargetPosition(Mathf::MultiplyFloat3(enemy->GetPosition(), direction));

        owner->SetMoveVecX(Camera::Instance().GetCameraDirection().x * 1000.0f);
        owner->SetMoveVecZ(Camera::Instance().GetCameraDirection().z * 1000.0f);
    }

    timer = 0.0f;
}

void InsectState::FlyingState::Execute(float elapsedTime)
{
    //owner->MoveToTarget(elapsedTime, owner->GetMoveSpeed());

    // ‰ñ“](‰¼)
    //owner->SetAngle({ owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z + 0.1f });
    //owner->Ratate({ 0.0f, 0.0f, 1.0f }, 1.0f);

    EnemyManager& enemyManager = EnemyManager::Instance();
    for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
    {
        for (int j = 0; j < enemyManager.GetEnemy(i)->GetParts().size(); j++)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);
            if (Collision::AttackNodeVsNode(
                owner, "NotFound", 1.0f,
                enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
                1.0f))
            {
                owner->SetClearVerocity();
                owner->SetPosition(owner->GetBeforePosition());
                owner->GetStateMachine()->ChangeState(Insect::State::Idle);
            }
        }
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_R)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);

    if (timer > 6.0f)
    {
        if (gamePad.GetButton() & GamePad::KEY_E)
            owner->GetStateMachine()->ChangeState(Insect::State::Flying);
    }

    if (timer > 24.0f)
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
    owner->SetTargetPosition({ player->GetPosition().x, 5.0f, player->GetPosition().z });
    DirectX::XMFLOAT3 targetPos = { player->GetPosition().x, 5.0f, player->GetPosition().z };
    DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(targetPos, owner->GetPosition());
  /*  owner->SetMoveVecX(lenght.x * 1000.0f);
    owner->SetMoveVecZ(lenght.z * 1000.0f);
    owner->SetVerocity(lenght);*/

    owner->MoveToTarget(elapsedTime, owner->GetMoveSpeed());

    // ‰ñ“](‰¼)
    //owner->SetAngle({ owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z + 0.01f });
    //owner->Ratate({ 0.0f, 0.0f, 1.0f }, 1.0f);

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E)
        owner->GetStateMachine()->ChangeState(Insect::State::Flying);
}

void InsectState::ReturnState::Exit()
{

}