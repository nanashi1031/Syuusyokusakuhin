#include "PlayerDerived.h"
#include "Player.h"
#include "Mathf.h"

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

// アイドルステートに入った時のメソッド
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
}
// アイドルステートで実行するメソッド
void IdleState::Execute(float elapsedTime)
{
	// スティックorキーボードでの移動があった場合歩きステートへ移動
	if (owner->GetMoveNow())
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}
}
// アイドルステートから出ていくときのメソッド
void IdleState::Exit()
{

}

// 徘徊ステートに入った時のメソッド
//void WanderState::Enter()
//{
//	owner->SetRandomTargetPosition();
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
//}
//
//// 徘徊ステートで実行するメソッド
//void WanderState::Execute(float elapsedTime)
//{
//	// 目的地点までのXZ平面での距離判定
//	DirectX::XMFLOAT3 position = owner->GetPosition();
//	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
//	float vx = targetPosition.x - position.x;
//	float vz = targetPosition.z - position.z;
//	float distSq = vx * vx + vz * vz;
//
//	// 目的地へ着いた
//	float radius = owner->GetRadius();
//	if (distSq < radius * radius)
//	{
//		// 待機ステートへ遷移
//		// ChangeStateクラスでStateを切り替える
//		// TODO 03_05 ChangeSubStateクラスで2層目のStateを切り替える
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Search::Idle));
//	}
//	// 目的地点へ移動
//	owner->MoveToTarget(elapsedTime, 0.5f);
//	// プレイヤー索敵
//	if (owner->SearchPlayer())
//	{
//		// 見つかったら追跡ステートへ遷移
//		// ChangeStateクラスでStateを切り替える
//		// TODO 03_05 ChangeStateクラスで1層目のStateを切り替える
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Battle));
//	}
//
//}

// 徘徊ステートから出ていくときのメソッド
//void WanderState::Exit()
//{
//
//}
//
//// 待機ステートに入った時のメソッド
//void IdleState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);
//	// タイマーをランダム設定
//	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
//}
//
//// 待機ステートで実行するメソッド
//void IdleState::Execute(float elapsedTime)
//{
//	//タイマー処理
//	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
//
//	// 待機時間が経過したとき徘徊ステートへ遷移
//	if (owner->GetStateTimer() < 0.0f)
//	{
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Search::Wander));
//	}
//
//	// プレイヤーが見つかったとき追跡ステートへ遷移
//	if (owner->SearchPlayer())
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Battle));
//}
//
//// 待機ステートから出ていくときのメソッド
//void IdleState::Exit()
//{
//
//}
//
//// 追跡ステートに入った時のメソッド
//void PursuitState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
//	// 数秒間追跡するタイマーをランダム設定
//	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
//}
//
//// 追跡ステートで実行するメソッド
//void PursuitState::Execute(float elapsedTime)
//{
//	// 目標地点をプレイヤー位置に設定
//	owner->SetTargetPosition(Player::Instance().GetPosition());
//
//	// 目的地点へ移動
//	owner->MoveToTarget(elapsedTime, 1.0);
//
//	//タイマー処理
//	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
//
//	// 追跡時間が経過したとき待機ステートへ遷移
//	if (owner->GetStateTimer() < 0.0f)
//	{
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Search));
//	}
//
//	//プレイヤーとの距離を計算
//	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
//	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
//	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
//	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
//
//	// 攻撃範囲に入ったとき攻撃ステートへ遷移
//	if (dist < owner->GetAttackRange())
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Battle::Attack));
//}
//
//// 追跡ステートから出ていくときのメソッド
//void PursuitState::Exit()
//{
//
//}
//
//// 攻撃ステートに入った時のメソッド
//void AttackState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Attack01), false);
//}
//
//// 攻撃ステートで実行するメソッド
//void AttackState::Execute(float elapsedTime)
//{
//	// 攻撃モーションが終了したとき追跡ステートへ移行
//	if (!owner->GetModel()->IsPlayAnimation())
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Battle::Pursuit));
//}
//
//// 攻撃ステートから出ていくときのメソッド
//void AttackState::Exit()
//{
//
//}