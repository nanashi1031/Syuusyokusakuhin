#include "EnemyPurpleDragonDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "CameraController.h"

void EnemyPurpleDragonState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::IdleNormal, true);
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
	else
	{
		int actIndex = static_cast<int>(Mathf::RandomRange(0.0f, 1.0f));

		switch (static_cast<IdleStateChange>(actIndex))
		{
		case IdleStateChange::Pursuit:
			// プレイヤーを追跡
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Pursuit);
			break;

		case IdleStateChange::fly:
			// 空を飛ぶ
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Fly);
			break;
		}
	}
}

void EnemyPurpleDragonState::IdleState::Exit()
{

}

void EnemyPurpleDragonState::BattleIdleState::Enter()
{
	stateTimer = 3.0f;

	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::BattleIdle, true);
}

void EnemyPurpleDragonState::BattleIdleState::Execute(float elapsedTime)
{
	// タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		// プレイヤー索敵
		Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
		if (enemy->SearchPlayer()) {
			// 見つかったら追跡ステートへ遷移
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Pursuit);
		}
		else
		{
			int actIndex = static_cast<int>(Mathf::RandomRange(0, 1));

			switch (static_cast<BattleIdleStateChange>(actIndex))
			{
			case BattleIdleStateChange::Pursuit:
				// プレイヤーを追跡
				owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Pursuit);
				break;

			case BattleIdleStateChange::fly:
				// 空を飛ぶ
				owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::TakeOff);
				break;
			}
		}
	}

	// ターゲット方向への進行ベクトルを算出
#if 0
	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(0)->GetPosition();
	owner->SetTargetPosition(target);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	owner->Turn(elapsedTime, vx, vz, 10.0f);
#endif
}


void EnemyPurpleDragonState::BattleIdleState::Exit()
{

}

void EnemyPurpleDragonState::HowlState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::Howl, false);
	SE_Howl1 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Howl.wav");
	seTimer = 0.0f;
	seFlag = false;
}

void EnemyPurpleDragonState::HowlState::Execute(float elapsedTime)
{
	if (!seFlag && seTimer > 0.7f)
	{
		float volume = 0.3f;
		SE_Howl1->Play(false, volume);
		seFlag = true;
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::BattleIdle);
	}

	seTimer += elapsedTime;
}

void EnemyPurpleDragonState::HowlState::Exit()
{

}

void EnemyPurpleDragonState::PursuitState::Enter()
{
	//タイマーをリセット
	stateTimer = 5.0f;
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::RunFound, true, 1.0f);
}

void EnemyPurpleDragonState::PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(0)->GetPosition();
	owner->SetTargetPosition(target);
	// 目標地点へ移動
	owner->MoveToTarget(elapsedTime, 20.0f);
	// エネミーの正面がおかしいので微調整
	DirectX::XMFLOAT3 angle = owner->GetAngle();
	float angleY = DirectX::XMConvertToDegrees(angle.y);
	angleY += 8.0f;
	angle.y = DirectX::XMConvertToRadians(angleY);
	owner->SetAngle(angle);

	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	// プレイヤーが索敵範囲から出ると
	if (dist > 0.0f)
	{
		// タイマーをマイナス
		stateTimer -= elapsedTime;
	}
	//プレイヤーが索敵範囲内なら
	else
	{
		//タイマーをリセット
		stateTimer = 5.0f;
	}

	if (dist < 15.0f)
	{
		int actIndex = static_cast<int>(Mathf::RandomRange(0, 2));

		switch (static_cast<PursuitStateChange>(actIndex))
		{
		case PursuitStateChange::BiteAttack:
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::BiteAttack);
			break;
		case PursuitStateChange::ClawAttack:
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::ClawAttack);
			break;
		}
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
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::Sleep, false);
}

void EnemyPurpleDragonState::NeglectState::Execute(float elapsedTime)
{
	// アニメーション再生が終わったら待機ステートへ移動
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}
}

void EnemyPurpleDragonState::NeglectState::Exit()
{

}

void EnemyPurpleDragonState::WalkState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::WalkFound, true);
}

void EnemyPurpleDragonState::WalkState::Execute(float elapsedTime)
{

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

}

void EnemyPurpleDragonState::RunState::Exit()
{

}

void EnemyPurpleDragonState::TakeOffState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::FryTakeOff, false);
	SE_Flapping = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Flapping.wav");
	float volume = 0.2f;
	SE_Flapping->Play(true, volume);

	stateTimer = 5.0f;
}

void EnemyPurpleDragonState::TakeOffState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Fly);
	}
}

void EnemyPurpleDragonState::TakeOffState::Exit()
{
	SE_Flapping->Stop();
}

void EnemyPurpleDragonState::FlyState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::FryFound, true);
	SE_Flapping = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Flapping.wav");
	float volume = 0.2f;
	SE_Flapping->Play(true, volume);

	stateTimer = 5.0f;
}

void EnemyPurpleDragonState::FlyState::Execute(float elapsedTime)
{
	// タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		int actIndex = static_cast<int>(Mathf::RandomRange(0, 1));

		switch (static_cast<FlyStateChange>(actIndex))
		{
		case FlyStateChange::Landing:
			// 着陸
			//owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Die);
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Landing);
			break;

		case FlyStateChange::TakeOff:
			// 空から攻撃
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::TakeOff);
			break;
		}
	}
}

void EnemyPurpleDragonState::FlyState::Exit()
{
	SE_Flapping->Stop();
}

void EnemyPurpleDragonState::LandingState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::FryLanding, false);
	SE_Flapping = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Flapping.wav");
}

void EnemyPurpleDragonState::LandingState::Execute(float elapsedTime)
{
	float volume = 0.2f;
	SE_Flapping->Play(true, volume);

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Howl);
	}
}

void EnemyPurpleDragonState::LandingState::Exit()
{
	SE_Flapping->Stop();
}

void EnemyPurpleDragonState::SleepState::Enter()
{
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::Animation::Sleep, true);
}

void EnemyPurpleDragonState::SleepState::Execute(float elapsedTime)
{

}

void EnemyPurpleDragonState::SleepState::Exit()
{

}

void EnemyPurpleDragonState::BiteAttackState::Enter()
{
	float animeSpeed = 1.0f;
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::Animation::AttackBiting, false);
	stateTimer = 0.0f;
	SE_Bite = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Bite.wav");
	seFlag = false;
}

void EnemyPurpleDragonState::BiteAttackState::Execute(float elapsedTime)
{
	Player* player = PlayerManager::Instance().GetPlayer(0);

	if (stateTimer > 0.5f)
	{
		float attackPower =
			Mathf::PlayerDamageCalculation(5.0f, player->GetDefensePower());
		DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
		int partsIndex = owner->SearchParts("Jaw1");
		if (partsIndex != -1)
		{
			// 当たり判定と被るため少し大きめに
			if (Collision::AttackNodeVsNode(
				owner, "Jaw1", owner->GetParts()[partsIndex].radius * 2.0f,
				player, "mixamorig:Hips", player->GetRadius(),
				hitPosition, attackPower))
			{

			}
			if (!seFlag)
			{
				float volume = 0.4f;
				SE_Bite->Play(false, volume);
				seFlag = true;
			}
		}
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// 0秒経ったら待機ステートへ移動
		if (stateTimer >= 0.0f)
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::BattleIdle);
		stateTimer += elapsedTime;
	}

	stateTimer += elapsedTime;
}

void EnemyPurpleDragonState::BiteAttackState::Exit()
{

}

void EnemyPurpleDragonState::ClawAttackState::Enter()
{
	float animeSpeed = 1.0f;
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::Animation::WingStrike, false);
	SE_Claw = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/Niren.wav");
	float volume = 0.4f;
	SE_Claw->Play(false, volume);

	stateTimer = 0.0f;
}

void EnemyPurpleDragonState::ClawAttackState::Execute(float elapsedTime)
{
	Player* player = PlayerManager::Instance().GetPlayer(0);

	float attackPower =
		Mathf::PlayerDamageCalculation(5.0f, player->GetDefensePower());
	DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
	int partsIndex = owner->SearchParts("WingClaw4_L");
	if (partsIndex != -1)
	{
		// 当たり判定と被るため少し大きめに
		if (Collision::AttackNodeVsNode(
			owner, "WingClaw4_L", owner->GetParts()[partsIndex].radius * 3.0f,
			player, "mixamorig:Hips", player->GetRadius(),
			hitPosition, attackPower))
		{
			DirectX::XMFLOAT3 shakePower = { 10.0f, 10.0f, 10.0f };
			CameraController::Instance().SetCamerarShake(true, shakePower);
		}
	}
	partsIndex = owner->SearchParts("WingClaw4_L_1");
	if (partsIndex != -1)
	{
		// 当たり判定と被るため少し大きめに
		if (Collision::AttackNodeVsNode(
			owner, "WingClaw4_L_1", owner->GetParts()[partsIndex].radius * 3.0f,
			player, "mixamorig:Hips", player->GetRadius(),
			hitPosition, attackPower))
		{
			DirectX::XMFLOAT3 shakePower = { 10.0f, 10.0f, 10.0f };
			CameraController::Instance().SetCamerarShake(true, shakePower);
		}
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// 0秒経ったら待機ステートへ移動
		if (stateTimer >= 0.0f)
			owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::BattleIdle);
		stateTimer += elapsedTime;
	}
}

void EnemyPurpleDragonState::ClawAttackState::Exit()
{

}

void EnemyPurpleDragonState::DamagesState::Enter()
{
	owner->GetModel()->PlayAnimation(
		EnemyPurpleDragon::Animation::Wince, false);
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
		EnemyPurpleDragon::Animation::Die, false, 1.0f, 0.5f);
	SE_FallDown =
		Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy/FallDown.wav");;
	seFlag = false;
	seTimer = 0.0f;
}

void EnemyPurpleDragonState::DieState::Execute(float elapsedTime)
{
	if (!seFlag && seTimer > 2.4f )
	{
		SE_FallDown->Play(false);
		seFlag = true;
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
		enemy->Destroy();
	}
	seTimer += elapsedTime;
}

void EnemyPurpleDragonState::DieState::Exit()
{

}