#include "Input\Input.h"
#include "PlayerDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"

// アクションステート
void ActionState::Enter()
{
	SetSubState(static_cast<int>(Player::Action::Idle));
}

// アクションステート実行するメソッド
void ActionState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// アクションステート出ていくときのメソッド
void ActionState::Exit()
{

}

// バトルステートに入った時のメソッド
void BattleState::Enter()
{
	SetSubState(static_cast<int>(Player::Battle::AttackCombo1));
}

// バトルステートで実行するメソッド
void BattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// バトルステートから出ていくときのメソッド
void BattleState::Exit()
{

}

// ダッシュステート
void DashState::Enter()
{
	SetSubState(static_cast<int>(Player::Dash::AttackDashu));
}

// ダッシュステート実行するメソッド
void DashState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// ダッシュステート出ていくときのメソッド
void DashState::Exit()
{

}

// 回避ステート
void AvoidState::Enter()
{
	SetSubState(static_cast<int>(Player::Avoid::Avoidance));
}

// 回避ステート実行するメソッド
void AvoidState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// 回避ステート出ていくときのメソッド
void AvoidState::Exit()
{

}

// アイドルステートに入った時のメソッド
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
	stateTimer = 0;
}

// アイドルステートで実行するメソッド
void IdleState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}
	// スティックorキーボードでの移動が10秒以上無い場合放置ステートへ移動
	else if(owner->GetMoveFlag() == 0.0f)
	{
		if (stateTimer > 10)
		{
			owner->GetStateMachine()->ChangeSubState(Player::Action::Neglect);
		}
		stateTimer += elapsedTime;
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}

	// 右クリック押されたら攻撃ステートへ遷移
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

// アイドルステートから出ていくときのメソッド
void IdleState::Exit()
{

}

// 放置ステートに入った時のメソッド
void NeglectState::Enter()
{
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

// 放置ステートで実行するメソッド
void NeglectState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}

	// アニメーション再生が終わったら待機ステートへ移動
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Idle);
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}
}

// 放置ステートから出ていくときのメソッド
void NeglectState::Exit()
{

}

// 歩きステートに入った時のメソッド
void WalkState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::WalkFront, true);
}

// 歩きステートで実行するメソッド
void WalkState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動をしてない場合待機ステートへ移動
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Idle);
	}

	// スティックorキーボードでの移動量が走り規定値以上なら
	if (owner->GetMoveFlag() >= 0.5f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Run);
	}

	// 左クリック押されたら攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}
}

// 歩きステートから出ていくときのメソッド
void WalkState::Exit()
{

}

// 走りステートに入った時のメソッド
void RunState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::RunFront, true);
}

// 走りステートで実行するメソッド
void RunState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動量が走り規定値より小さいなら
	if (owner->GetMoveFlag() < 0.5f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}

	// 左クリック押されたらダッシュ攻撃ステートへ遷移
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Dash);
	}
}

// 走りステートから出ていくときのメソッド
void RunState::Exit()
{

}

// コンボ攻撃1ステートに入った時のメソッド
void AttackCombo1State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKesakiri, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// コンボ攻撃1ステートで実行するメソッド
void AttackCombo1State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::IntersectNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 10.0f,
			10.0f, 10.0f);
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
			owner->GetStateMachine()->ChangeSubState(Player::Battle::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 0秒経ったら待機ステートへ移動
			if (stateTimer >= 0.0f)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

// コンボ攻撃1ステートから出ていくときのメソッド
void AttackCombo1State::Exit()
{

}

// コンボ攻撃2ステートに入った時のメソッド
void AttackCombo2State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// コンボ攻撃2ステートで実行するメソッド
void AttackCombo2State::Execute(float elapsedTime)
{
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
			owner->GetStateMachine()->ChangeSubState(Player::Battle::AttackCombo3);
		}
		else if (!nextAttackFlag)
		{
			// 1秒経ったら待機ステートへ移動
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

// コンボ攻撃2ステートから出ていくときのメソッド
void AttackCombo2State::Exit()
{

}

// コンボ攻撃3ステートに入った時のメソッド
void AttackCombo3State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, 2.0f);
}

// コンボ攻撃3ステートで実行するメソッド
void AttackCombo3State::Execute(float elapsedTime)
{
	// アニメーション再生が終わったら
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

// コンボ攻撃3ステートから出ていくときのメソッド
void AttackCombo3State::Exit()
{

}

// ダッシュ攻撃ステートに入った時のメソッド
void AttackDashuState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// ダッシュ攻撃ステートで実行するメソッド
void AttackDashuState::Execute(float elapsedTime)
{
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
			owner->GetStateMachine()->ChangeSubState(Player::Battle::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 1秒経ったら待機ステートへ移動
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

// ダッシュ攻撃ステートから出ていくときのメソッド
void AttackDashuState::Exit()
{

}

// 回避ステートに入った時のメソッド
void AvoiDanceState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Block, false);
}

// 回避ステートで実行するメソッド
void AvoiDanceState::Execute(float elapsedTime)
{
	// アニメーション再生が終了時
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

// 回避ステートから出ていくときのメソッド
void AvoiDanceState::Exit()
{

}