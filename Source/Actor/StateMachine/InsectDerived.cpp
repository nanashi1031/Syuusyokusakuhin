#include <memory>
#include "Input\Input.h"
#include "InsectDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "Camera.h"
#include "CameraController.h"
#include "LightManager.h"
#include "Extract.h"

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
    if (length.x < 1.0f && length.z < 1.0f)
    {
        owner->GetStateMachine()->ChangeState(Insect::State::Pursuit);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E ||
        gamePad.GetButton() & GamePad::BTN_PAD_LB)
        if (CameraController::Instance().GetLockOnFlag())
            owner->GetStateMachine()->ChangeState(Insect::State::Flying);

    if (gamePad.GetButton() & GamePad::KEY_R ||
        gamePad.GetButton() & GamePad::BTN_PAD_LT)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);
}

void InsectState::IdleState::Exit()
{
    owner->SetClearVerocity();
}

void InsectState::PursuitState::Enter()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());
    if (owner->GetExtractColor() == ExtractColor::Heal)
    {
        int he = player->GetHealth() + 30;
        if (he > player->GetMaxHealth()) he = player->GetMaxHealth();
        player->SetHealth(he);
    }
    else if (owner->GetExtractColor() != ExtractColor::None)
    {
        Extract::Instance().SetExtract(owner->GetExtractColor());
        player->SetExtractColor(owner->GetExtractColor());
    }

    if (owner->GetLightIndex() >= 0)
    {
        LightManager::Instane().RemoveIndex(owner->GetLightIndex());
        owner->SetLightIndex(-1);
        owner->SetExtractColor(-1);
    }
}

void InsectState::PursuitState::Execute(float elapsedTime)
{
    Player* player =
        PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex());
    owner->SetPosition(player->GetPosition());

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E ||
        gamePad.GetButton() & GamePad::BTN_PAD_LB)
        if (CameraController::Instance().GetLockOnFlag())
            owner->GetStateMachine()->ChangeState(Insect::State::Flying);
}

void InsectState::PursuitState::Exit()
{
    owner->SetClearVerocity();
}

void InsectState::FlyingState::Enter()
{
    CameraController& cameraController = CameraController::Instance();
    EnemyManager& enemyManager = EnemyManager::Instance();
    if (cameraController.GetLockOnFlag())
    {
        Enemy* enemy = enemyManager.GetEnemy(0);
        int targetIndex =
            cameraController.GetTargets()[cameraController.GetTagetIndex()].index;
        // ƒ^[ƒQƒbƒg‰ÓŠ‚Ö”ò‚Ô‚æ‚¤‚É
        Model::Node* node = enemy->GetModel()->FindNode(
            enemy->GetParts()[targetIndex].name);
        DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(
            enemy->GetNodePosition(node), owner->GetPosition());
        owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));
    }
    else
    {
        Enemy* enemy = enemyManager.GetEnemy(0);

        DirectX::XMFLOAT3 direction =
            Mathf::MultiplyFloat3Float(Camera::Instance().GetCameraDirection(), 1.0f);
        direction.y = 0.0f;
        owner->SetTargetPosition(Mathf::MultiplyFloat3(enemy->GetPosition(), direction));
        DirectX::XMFLOAT3 destination =
            Mathf::MultiplyFloat3Float(direction, owner->GetMoveSpeed());
        DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(
            destination, owner->GetPosition());
        owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));
    }

    timer = 0.0f;
}

void InsectState::FlyingState::Execute(float elapsedTime)
{
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
                owner->SetExtractColor(enemy->GetParts()[j].extractColor);
                DirectX::XMFLOAT4 color =
                    Extract::Instance().ColorConversion(owner->GetExtractColor());

                if (owner->GetLightIndex() >= 0)
                {
                    LightManager::Instane().RemoveIndex(owner->GetLightIndex());
                }

                Light* light = new Light(LightType::Point);
                light->SetPosition(owner->GetPosition());
                light->SetColor(DirectX::XMFLOAT4(color));
                light->SetRange(2.0f);
                LightManager::Instane().Register(light);
                owner->SetLightIndex(LightManager::Instane().GetLightCount());

                owner->SetPosition(owner->GetBeforePosition());
                owner->GetStateMachine()->ChangeState(Insect::State::Idle);
            }
        }
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_R ||
        gamePad.GetButton() & GamePad::BTN_PAD_LT)
        owner->GetStateMachine()->ChangeState(Insect::State::Return);

    if (timer > 3.0f)
    {
        if (gamePad.GetButton() & GamePad::KEY_E ||
            gamePad.GetButton() & GamePad::BTN_PAD_LB)
            if (CameraController::Instance().GetLockOnFlag())
                owner->GetStateMachine()->ChangeState(Insect::State::Flying);
    }

    if (timer > 18.0f)
    {
        owner->GetStateMachine()->ChangeState(Insect::State::Idle);
    }

    timer += 0.1f;
}

void InsectState::FlyingState::Exit()
{
    owner->SetClearVerocity();
}

void InsectState::ReturnState::Enter()
{

}

void InsectState::ReturnState::Execute(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();
    Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());
    DirectX::XMFLOAT3 targetPos = player->GetPosition();
    targetPos.y += 2.0f;
    DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(targetPos, owner->GetPosition());
    owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));


    // ‰ñ“](‰¼)
    //owner->SetAngle({ owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z + 0.01f });
    //owner->Ratate({ 0.0f, 0.0f, 1.0f }, 1.0f);

    DirectX::XMFLOAT3 length =
        Mathf::SubtractFloat3(owner->GetPosition(), player->GetPosition());
    length = Mathf::MakePlusFloat3(length);
    if (length.x < 1.0f && length.z < 1.0f)
    {
        owner->GetStateMachine()->ChangeState(Insect::State::Pursuit);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::KEY_E ||
        gamePad.GetButton() & GamePad::BTN_PAD_LB)
        if (CameraController::Instance().GetLockOnFlag())
            owner->GetStateMachine()->ChangeState(Insect::State::Flying);
}

void InsectState::ReturnState::Exit()
{
    owner->SetClearVerocity();
}