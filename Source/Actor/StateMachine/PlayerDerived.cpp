#include "Input\Input.h"
#include "PlayerDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"

void ActionState::Enter()
{
	SetSubState(static_cast<int>(Player::Action::Idle));
}

void ActionState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void ActionState::Exit()
{

}

void BattleState::Enter()
{
	SetSubState(static_cast<int>(Player::Battle::AttackCombo1));
}

void BattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void BattleState::Exit()
{

}

void DashState::Enter()
{
	SetSubState(static_cast<int>(Player::Dash::AttackDashu));
}

void DashState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void DashState::Exit()
{

}

void AvoidState::Enter()
{
	SetSubState(static_cast<int>(Player::Avoid::Avoidance));
}

void AvoidState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void AvoidState::Exit()
{

}

void DamageState::Enter()
{
	SetSubState(static_cast<int>(Player::Damage::Damages));
}

void DamageState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void DamageState::Exit()
{

}

void DeathState::Enter()
{
	SetSubState(static_cast<int>(Player::Death::Die));
}

void DeathState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

void DeathState::Exit()
{

}

void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
	stateTimer = 0;
}

void IdleState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ���10�ȏ㖳���ꍇ���u�X�e�[�g�ֈړ�
	else if(owner->GetMoveFlag() == 0.0f)
	{
		if (stateTimer > 10)
		{
			owner->GetStateMachine()->ChangeSubState(Player::Action::Neglect);
		}
		stateTimer += elapsedTime;
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

void IdleState::Exit()
{

}

void NeglectState::Enter()
{
	// �����_���ŕ��u�A�j���[�V�������Đ�
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

void NeglectState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}

	// �A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�ֈړ�
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Idle);
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

void NeglectState::Exit()
{

}

void WalkState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::WalkFront, true);
}

void WalkState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ������ĂȂ��ꍇ�ҋ@�X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Idle);
	}

	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l�ȏ�Ȃ�
	if (owner->GetMoveFlag() >= 0.5f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Run);
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Battle);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

void WalkState::Exit()
{

}

void RunState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::RunFront, true);
}

void RunState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l��菬�����Ȃ�
	if (owner->GetMoveFlag() < 0.5f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}

	// ���N���b�N�����ꂽ��_�b�V���U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Dash);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

void RunState::Exit()
{

}

void AttackCombo1State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKesakiri, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void AttackCombo1State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::IntersectNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// ���N���b�N������U���t���O������
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeSubState(Player::Battle::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 0�b�o������ҋ@�X�e�[�g�ֈړ�
			if (stateTimer >= 0.0f)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

void AttackCombo1State::Exit()
{

}

void AttackCombo2State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void AttackCombo2State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::IntersectNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// ���N���b�N������U���t���O������
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeSubState(Player::Battle::AttackCombo3);
		}
		else if (!nextAttackFlag)
		{
			// 1�b�o������ҋ@�X�e�[�g�ֈړ�
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

void AttackCombo2State::Exit()
{

}

void AttackCombo3State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, 2.0f);
}

void AttackCombo3State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::IntersectNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			10.0f);
	}

	// �A�j���[�V�����Đ����I�������
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

void AttackCombo3State::Exit()
{

}

void AttackDashuState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false, 0.0f, 1.5f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

void AttackDashuState::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Collision::IntersectNodeVsNode(
			owner, "mixamorig:Sword_joint", 1.0f,
			enemyManager.GetEnemy(i), "Hand_L", 3.0f,
			5.0f);
	}

	// ���N���b�N������U���t���O������
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::Battle);
		}
		else if (!nextAttackFlag)
		{
			// 1�b�o������ҋ@�X�e�[�g�ֈړ�
			owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

void AttackDashuState::Exit()
{

}

void AvoiDanceState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Block, true);
}

void AvoiDanceState::Execute(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	// �}�E�X���E�N���b�N���Ă���Ԃ͉���A��������Action�X�e�[�g��
	if (mouse.GetButtonUp() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

void AvoiDanceState::Exit()
{

}

void DamagesState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::HitSmall, false);
}

void DamagesState::Execute(float elapsedTime)
{
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

void DamagesState::Exit()
{

}

void DieState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::DeathFront, false);
}

void DieState::Execute(float elapsedTime)
{
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

void DieState::Exit()
{

}