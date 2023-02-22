#include "Input\Input.h"
#include "EnemyPurpleDragonDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "LightManager.h"

void EnemyPurpleDragonState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::IdleNormal, true);
	stateTimer = 0;
}

void EnemyPurpleDragonState::IdleState::Execute(float elapsedTime)
{
	// タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//TransitionPursuitState();
	}
	// プレイヤー索敵
	Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
	if (enemy->SearchPlayer()) {
		// 見つかったら追跡ステートへ遷移
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Pursuit);
	}
}

void EnemyPurpleDragonState::IdleState::Exit()
{

}

void EnemyPurpleDragonState::PursuitState::Enter()
{
	//タイマーをリセット
	stateTimer = 10.0f;
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::RunFound, true);
}

void EnemyPurpleDragonState::PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	owner->SetTargetPosition(PlayerManager::Instance().GetPlayer(0)->GetPosition());
	// 目標地点へ移動
	owner->MoveToTarget(elapsedTime, 5.0f);

	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	// プレイヤーが索敵範囲から出ると
	if (dist > 10.0f)
	{
		// タイマーをマイナス
		stateTimer -= elapsedTime;
	}
	//プレイヤーが索敵範囲内なら
	else
	{
		//タイマーをリセット
		stateTimer = 10.0f;
	}

	if (dist < 3.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackCombo);
	}
	//タイマーが0になると
	if (stateTimer < 0.0f)
	{
		// 待機ステートへ遷移
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}
}

void EnemyPurpleDragonState::PursuitState::Exit()
{

}

void EnemyPurpleDragonState::NeglectState::Enter()
{
	// ランダムで放置アニメーションを再生
	int neglectIndex = static_cast<int>(Mathf::RandomRange(0, 2));
	switch (neglectIndex)
	{
	case 0:
		/*owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::Neglect1, false);
		break;
	case 1:
		owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::Neglect2, false);
		break;
	case 2:
		owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::Neglect3, false);*/
		break;
	}
}

void EnemyPurpleDragonState::NeglectState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Walk);
	}

	// アニメーション再生が終わったら待機ステートへ移動
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_PAD_RB))
	{
		//owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackCombo1);
	}

	// 右クリック押されたら回避ステートへ遷移
	/*if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Avoidance);
	}*/
}

void EnemyPurpleDragonState::NeglectState::Exit()
{

}

void EnemyPurpleDragonState::WalkState::Enter()
{
	//owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::WalkFront, true);
}

void EnemyPurpleDragonState::WalkState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動をしてない場合待機ステートへ移動
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}

	// スティックorキーボードでの移動量が走り規定値以上なら
	if (owner->GetMoveFlag() >= 0.5f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Run);
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_PAD_RB))
	{
		//owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackCombo1);
	}

	// 右クリック押されたら回避ステートへ遷移
	/*if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Avoidance);
	}*/
}

void EnemyPurpleDragonState::WalkState::Exit()
{

}

void EnemyPurpleDragonState::RunState::Enter()
{
	//owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::RunFront, true);
}

void EnemyPurpleDragonState::RunState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動量が走り規定値より小さいなら
	if (owner->GetMoveFlag() < 0.5f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Walk);
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_PAD_RB))
	{
		//owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackCombo1);
		//owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackDashu);
	}

	// 右クリック押されたら回避ステートへ遷移
	/*if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Avoidance);
	}*/
}

void EnemyPurpleDragonState::RunState::Exit()
{

}

void EnemyPurpleDragonState::AttackCombo1State::Enter()
{
	float animeSpeed = 1.0f;
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::EnemyPurpleDragonAnimation::WingStrike, false, 1.0f, animeSpeed);
	stateTimer = 0.0f;
	nextAttackFlag = false;
	owner->SetMovingFlag(false);
}

void EnemyPurpleDragonState::AttackCombo1State::Execute(float elapsedTime)
{
	Player* player = PlayerManager::Instance().GetPlayer(0);
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		for (int j = 0; j < enemy->GetParts().size(); j++)
		{
			float attackPower =
				Mathf::PlayerDamageCalculation(10.0f, 1.0f);
			if (Collision::AttackNodeVsNode(
				owner, "Wing3_L", 2.0f,
				player, "mixamorig:Hips", player->GetRadius(),
				attackPower))
			{
				if (owner->GetLightIndex() < 0)
				{
					DirectX::XMFLOAT4 color =
						Extract::Instance().ColorConversion(enemy->GetParts()[j].extractColor);
					Light* light = new Light(LightType::Point);
					DirectX::XMFLOAT3 LightPosition =
						owner->GetNodePosition(owner->GetNode("Wing3_L"));
					light->SetPosition(LightPosition);
					light->SetColor(DirectX::XMFLOAT4({ color }));
					light->SetRange(2.0f);
					LightManager::Instane().Register(light);
					owner->SetLightIndex(LightManager::Instane().GetLightCount());
				}
				//owner->GetAudios(0)->Play(false);
			}
		}
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_PAD_RB))
	{
		nextAttackFlag = true;
	}

	// アニメーション再生が終了時
	//if (!owner->GetModel()->IsPlayAnimation())
	float nextAnimeSeconds = 0.9f;
	if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
	{
		// 0秒経ったら待機ステートへ移動
		if (stateTimer >= 0.0f)
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
		stateTimer += elapsedTime;
	}
}

void EnemyPurpleDragonState::AttackCombo1State::Exit()
{
	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void EnemyPurpleDragonState::DamagesState::Enter()
{
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::EnemyPurpleDragonAnimation::AttackBiting, false);
}

void EnemyPurpleDragonState::DamagesState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}
}

void EnemyPurpleDragonState::DamagesState::Exit()
{

}

void EnemyPurpleDragonState::DieState::Enter()
{
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::EnemyPurpleDragonAnimation::Die, false);
}

void EnemyPurpleDragonState::DieState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
		enemy->Destroy();
	}
}

void EnemyPurpleDragonState::DieState::Exit()
{

}