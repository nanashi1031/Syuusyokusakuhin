#include "StateDerived.h"
#include "PlayerManager.h"
#include "Mathf.h"

SearchState::~SearchState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void SearchState::Enter()
{
	SetSubState(static_cast<int>(EnemySlime::Search::Idle));
}
// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void SearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void SearchState::Exit()
{
}

// TODO 03_03 HierarchicalState���p�������e�X�e�[�g��
// BattleState�N���X�̃����o�֐��̒�`���s���Ȃ����B
// �T�[�`�X�e�[�g�f�X�g���N�^
BattleState::~BattleState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �o�g���X�e�[�g�ɓ��������̃��\�b�h
void BattleState::Enter()
{
	SetSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
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

// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		// TODO 03_05 ChangeSubState�N���X��2�w�ڂ�State��؂�ւ���
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		// TODO 03_05 ChangeState�N���X��1�w�ڂ�State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Battle));
	}

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

// �ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::IdleNormal), true);
	// �^�C�}�[�������_���ݒ�
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
	//�^�C�}�[����
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J��
	if (owner->GetStateTimer() < 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Wander));
	}

	// �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J��
	if (owner->SearchPlayer())
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Battle));
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit()
{

}

// �ǐՃX�e�[�g�ɓ��������̃��\�b�h
void PursuitState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::RunFWD), true);
	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}

// �ǐՃX�e�[�g�Ŏ��s���郁�\�b�h
void PursuitState::Execute(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	owner->SetTargetPosition(PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition());

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 1.0);

	//�^�C�}�[����
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J��
	if (owner->GetStateTimer() < 0.0f)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}

	//�v���C���[�Ƃ̋������v�Z
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J��
	if (dist < owner->GetAttackRange())
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Attack));
}

// �ǐՃX�e�[�g����o�Ă����Ƃ��̃��\�b�h
void PursuitState::Exit()
{

}

// �U���X�e�[�g�ɓ��������̃��\�b�h
void AttackState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyAnimation::Attack01), false);
}

// �U���X�e�[�g�Ŏ��s���郁�\�b�h
void AttackState::Execute(float elapsedTime)
{
	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetModel()->IsPlayAnimation())
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
}

// �U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackState::Exit()
{

}