#include "PlayerDerived.h"
#include "Player.h"
#include "Mathf.h"

// �A�N�V�����X�e�[�g
void ActionState::Enter()
{
	SetSubState(static_cast<int>(Player::Action::Idle));
}

// �A�N�V�����X�e�[�g���s���郁�\�b�h
void ActionState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// �A�N�V�����X�e�[�g�o�Ă����Ƃ��̃��\�b�h
void ActionState::Exit()
{

}

// �o�g���X�e�[�g�ɓ��������̃��\�b�h
void BattleState::Enter()
{

}
// �o�g���X�e�[�g�Ŏ��s���郁�\�b�h
void BattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �o�g���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BattleState::Exit()
{

}

// �A�C�h���X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
}
// �A�C�h���X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveNow())
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}
}
// �A�C�h���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit()
{

}

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
//void WanderState::Enter()
//{
//	owner->SetRandomTargetPosition();
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
//}
//
//// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
//void WanderState::Execute(float elapsedTime)
//{
//	// �ړI�n�_�܂ł�XZ���ʂł̋�������
//	DirectX::XMFLOAT3 position = owner->GetPosition();
//	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
//	float vx = targetPosition.x - position.x;
//	float vz = targetPosition.z - position.z;
//	float distSq = vx * vx + vz * vz;
//
//	// �ړI�n�֒�����
//	float radius = owner->GetRadius();
//	if (distSq < radius * radius)
//	{
//		// �ҋ@�X�e�[�g�֑J��
//		// ChangeState�N���X��State��؂�ւ���
//		// TODO 03_05 ChangeSubState�N���X��2�w�ڂ�State��؂�ւ���
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Search::Idle));
//	}
//	// �ړI�n�_�ֈړ�
//	owner->MoveToTarget(elapsedTime, 0.5f);
//	// �v���C���[���G
//	if (owner->SearchPlayer())
//	{
//		// ����������ǐՃX�e�[�g�֑J��
//		// ChangeState�N���X��State��؂�ւ���
//		// TODO 03_05 ChangeState�N���X��1�w�ڂ�State��؂�ւ���
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Battle));
//	}
//
//}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//void WanderState::Exit()
//{
//
//}
//
//// �ҋ@�X�e�[�g�ɓ��������̃��\�b�h
//void IdleState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);
//	// �^�C�}�[�������_���ݒ�
//	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
//}
//
//// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
//void IdleState::Execute(float elapsedTime)
//{
//	//�^�C�}�[����
//	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
//
//	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J��
//	if (owner->GetStateTimer() < 0.0f)
//	{
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Search::Wander));
//	}
//
//	// �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J��
//	if (owner->SearchPlayer())
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Battle));
//}
//
//// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//void IdleState::Exit()
//{
//
//}
//
//// �ǐՃX�e�[�g�ɓ��������̃��\�b�h
//void PursuitState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
//	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
//	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
//}
//
//// �ǐՃX�e�[�g�Ŏ��s���郁�\�b�h
//void PursuitState::Execute(float elapsedTime)
//{
//	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
//	owner->SetTargetPosition(Player::Instance().GetPosition());
//
//	// �ړI�n�_�ֈړ�
//	owner->MoveToTarget(elapsedTime, 1.0);
//
//	//�^�C�}�[����
//	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
//
//	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J��
//	if (owner->GetStateTimer() < 0.0f)
//	{
//		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Search));
//	}
//
//	//�v���C���[�Ƃ̋������v�Z
//	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
//	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
//	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
//	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
//
//	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J��
//	if (dist < owner->GetAttackRange())
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Battle::Attack));
//}
//
//// �ǐՃX�e�[�g����o�Ă����Ƃ��̃��\�b�h
//void PursuitState::Exit()
//{
//
//}
//
//// �U���X�e�[�g�ɓ��������̃��\�b�h
//void AttackState::Enter()
//{
//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Attack01), false);
//}
//
//// �U���X�e�[�g�Ŏ��s���郁�\�b�h
//void AttackState::Execute(float elapsedTime)
//{
//	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
//	if (!owner->GetModel()->IsPlayAnimation())
//		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::Battle::Pursuit));
//}
//
//// �U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//void AttackState::Exit()
//{
//
//}