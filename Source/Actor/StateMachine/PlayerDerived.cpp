#include "Input\Input.h"
#include "PlayerDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"

void PlayerState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
	stateTimer = 0;
}

void PlayerState::IdleState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}
	// スティックorキーボードでの移動が10以上無い場合放置ステートへ移動
	else if(owner->GetMoveFlag() == 0.0f)
	{
		if (stateTimer > 10)
		{
			owner->GetStateMachine()->ChangeState(Player::State::Neglect);
		}
		stateTimer += elapsedTime;
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// 右クリック押されたら回避ステートへ遷移
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
	}
}

void PlayerState::IdleState::Exit()
{

}

void PlayerState::NeglectState::Enter()
{
	// ランダムで放置アニメーションを再生
	int neglectIndex = static_cast<int>(Mathf::RandomRange(0, 2));
	switch (neglectIndex)
	{
	case 0:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect1, false);
		break;
	case 1:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect2, false);
		break;
	case 2:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect3, false);
		break;
	}
}

void PlayerState::NeglectState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}

	// アニメーション再生が終わったら待機ステートへ移動
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// 右クリック押されたら回避ステートへ遷移
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
	}
}

void PlayerState::NeglectState::Exit()
{

}

void PlayerState::WalkState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::WalkFront, true);
}

void PlayerState::WalkState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動をしてない場合待機ステートへ移動
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}

	// スティックorキーボードでの移動量が走り規定値以上なら
	if (owner->GetMoveFlag() >= 0.5f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Run);
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// 右クリック押されたら回避ステートへ遷移
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
	}
}

void PlayerState::WalkState::Exit()
{

}

void PlayerState::RunState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::RunFront, true);
}

void PlayerState::RunState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動量が走り規定値より小さいなら
	if (owner->GetMoveFlag() < 0.5f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}

	// 左クリック押されたらダッシュ攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackDashu);
	}

	// 右クリック押されたら回避ステートへ遷移
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
	}
}

void PlayerState::RunState::Exit()
{

}

void PlayerState::AttackCombo1State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKesakiri, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void PlayerState::AttackCombo1State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::AttackNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// 左クリックしたら攻撃フラグが建つ
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// 攻撃フラグがtrueなら
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 0秒経ったら待機ステートへ移動
			if (stateTimer >= 0.0f)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackCombo1State::Exit()
{

}

void PlayerState::AttackCombo2State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void PlayerState::AttackCombo2State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::AttackNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// 左クリックしたら攻撃フラグが建つ
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// 攻撃フラグがtrueなら
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo3);
		}
		else if (!nextAttackFlag)
		{
			// 1秒経ったら待機ステートへ移動
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackCombo2State::Exit()
{

}

void PlayerState::AttackCombo3State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, 2.0f);
}

void PlayerState::AttackCombo3State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::AttackNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			10.0f);
	}

	// アニメーション再生が終わったら
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::AttackCombo3State::Exit()
{

}

void PlayerState::AttackDashuState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false, 0.0f, 1.5f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void PlayerState::AttackDashuState::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::AttackNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// 左クリックしたら攻撃フラグが建つ
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// 攻撃フラグがtrueなら
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 1秒経ったら待機ステートへ移動
			owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackDashuState::Exit()
{

}

void PlayerState::AvoiDanceState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Block, true);
}

void PlayerState::AvoiDanceState::Execute(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	// マウスを右クリックしている間は回避、離したらActionステートへ
	if (mouse.GetButtonUp() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::AvoiDanceState::Exit()
{

}

void PlayerState::DamagesState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::HitSmall, false);
}

void PlayerState::DamagesState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::DamagesState::Exit()
{

}

void PlayerState::DieState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::DeathFront, false);
}

void PlayerState::DieState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::DieState::Exit()
{

}