#include <memory>
#include "Input.h"
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

    if (owner->GetExtractColor() != ExtractColor::None)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
          DirectX::XMFLOAT3 pursuitPosition =
              enemy->GetNodePosition(enemy->GetNode(owner->GetPursuitName()));
          DirectX::XMFLOAT3 i = Mathf::AddFloat3(pursuitPosition, owner->GetPursuitLength());
          owner->SetPosition(Mathf::AddFloat3(pursuitPosition, owner->GetPursuitLength()));
    }

    DirectX::XMFLOAT3 length =
        Mathf::SubtractFloat3(owner->GetPosition(), player->GetPosition());
    length = Mathf::MakePlusFloat3(length);
    // プレイヤーの近くにいるなら
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
        SE_Heal = Audio::Instance().LoadAudioSource("Data/Audio/SE/Insect/Whistle.wav");
        SE_Heal->Play(false);

        float healHealth = player->GetHealth() + owner->GetRecoveryAmount();
        if (healHealth > player->GetMaxHealth()) healHealth = player->GetMaxHealth();
        player->SetHealth(healHealth);
    }
    else if (owner->GetExtractColor() != ExtractColor::None)
    {
        SE_ExtractGet = Audio::Instance().LoadAudioSource("Data/Audio/SE/Insect/ExtractGet.wav");
        SE_ExtractGet->Play(false);

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
    Enemy* enemy = enemyManager.GetEnemy(0);

    if (cameraController.GetLockOnFlag())
    {
        int targetIndex =
            cameraController.GetTargets()[cameraController.GetTagetIndex()].index;
        // ターゲット箇所へ飛ぶように
        Model::Node* node = enemy->GetModel()->FindNode(
            enemy->GetParts()[targetIndex].name);
        DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(
            enemy->GetNodePosition(node), owner->GetPosition());
        owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));
    }
    else
    {
        // 現在封印中、余裕ができたら
#if 0
        DirectX::XMFLOAT3 direction =
            Mathf::MultiplyFloat3Float(Camera::Instance().GetCameraDirection(), 1.0f);
        direction.y = 0.0f;
        owner->SetTargetPosition(Mathf::MultiplyFloat3(enemy->GetPosition(), direction));
        DirectX::XMFLOAT3 destination =
            Mathf::MultiplyFloat3Float(direction, owner->GetMoveSpeed());
        DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(
            destination, owner->GetPosition());
        owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));
#endif
    }

    SE_Whistle = Audio::Instance().LoadAudioSource("Data/Audio/SE/Insect/Whistle.wav");
    SE_Whistle->Play(false);
    SE_Hit = Audio::Instance().LoadAudioSource("Data/Audio/SE/Insect/Hit.wav");
    timer = 0.0f;
}

void InsectState::FlyingState::Execute(float elapsedTime)
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
    {
        for (int j = 0; j < enemyManager.GetEnemy(i)->GetParts().size(); j++)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);
            float attackPower = 1.0f;
            float invincibleTime = 1.0f;
            float blowingPower = 0.0f;
            DirectX::XMFLOAT3 hitPosition = {0.0f, 0.0f, 0.0f};
            // 当たりやすくするため大きめに当たり判定をとる
            if (Collision::AttackNodeVsNode(
                owner, "NotFound", owner->GetRadius() * 2,
                enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
                hitPosition, attackPower, invincibleTime, blowingPower))
            {
                owner->SetExtractColor(enemy->GetParts()[j].extractColor);
                DirectX::XMFLOAT4 color =
                    Extract::Instance().ColorConversion(owner->GetExtractColor());

                if (owner->GetLightIndex() >= 0)
                {
                    LightManager::Instane().RemoveIndex(owner->GetLightIndex());
                }

                owner->SetPursuitName(enemy->GetParts()[j].name);
                DirectX::XMFLOAT3 nodePosition =
                    enemy->GetNodePosition(enemy->GetNode(enemy->GetParts()[j].name));
                owner->SetPursuitLength(Mathf::CalculateLength(nodePosition, hitPosition));

                Light* light = new Light(LightType::Point);
                light->SetPosition(owner->GetPosition());
                light->SetColor(DirectX::XMFLOAT4(color));
                light->SetRange(2.0f);
                LightManager::Instane().Register(light);
                owner->SetLightIndex(LightManager::Instane().GetLightCount());

                owner->SetPosition(owner->GetBeforePosition());
                owner->GetStateMachine()->ChangeState(Insect::State::Idle);

                SE_Hit->Play(false);
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

    // 10秒以上何にも当たらなければアイドルステートへ
    float limitTime = 10.0f;
    if (timer > limitTime)
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
    // プレイヤーの少し上を目標地とする
    targetPos.y += 2.0f;
    DirectX::XMFLOAT3 lenght = Mathf::CalculateLength(targetPos, owner->GetPosition());
    owner->SetVerocity(Mathf::MultiplyFloat3Float(lenght, owner->GetMoveSpeed()));

    DirectX::XMFLOAT3 length =
        Mathf::SubtractFloat3(owner->GetPosition(), player->GetPosition());
    length = Mathf::MakePlusFloat3(length);
    // プレイヤーの近くにいるなら
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