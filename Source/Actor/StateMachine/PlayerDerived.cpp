#include "Input\Input.h"
#include "PlayerDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"

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
	SetSubState(static_cast<int>(Player::Battle::AttackCombo1));
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

// �_�b�V���X�e�[�g
void DashState::Enter()
{
	SetSubState(static_cast<int>(Player::Dash::AttackDashu));
}

// �_�b�V���X�e�[�g���s���郁�\�b�h
void DashState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// �_�b�V���X�e�[�g�o�Ă����Ƃ��̃��\�b�h
void DashState::Exit()
{

}

// ����X�e�[�g
void AvoidState::Enter()
{
	SetSubState(static_cast<int>(Player::Avoid::Avoidance));
}

// ����X�e�[�g���s���郁�\�b�h
void AvoidState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}

// ����X�e�[�g�o�Ă����Ƃ��̃��\�b�h
void AvoidState::Exit()
{

}

// �A�C�h���X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
	stateTimer = 0;
}

// �A�C�h���X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeSubState(Player::Action::Walk);
	}
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ���10�b�ȏ㖳���ꍇ���u�X�e�[�g�ֈړ�
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

	// �E�N���b�N�����ꂽ��U���X�e�[�g�֑J��
	if (mouse.GetButtonDown() & mouse.BTN_RIGHT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Avoid);
	}
}

// �A�C�h���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit()
{

}

// ���u�X�e�[�g�ɓ��������̃��\�b�h
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

// ���u�X�e�[�g�Ŏ��s���郁�\�b�h
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
}

// ���u�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void NeglectState::Exit()
{

}

// �����X�e�[�g�ɓ��������̃��\�b�h
void WalkState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::WalkFront, true);
}

// �����X�e�[�g�Ŏ��s���郁�\�b�h
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
}

// �����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WalkState::Exit()
{

}

// ����X�e�[�g�ɓ��������̃��\�b�h
void RunState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::RunFront, true);
}

// ����X�e�[�g�Ŏ��s���郁�\�b�h
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
}

// ����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void RunState::Exit()
{

}

// �R���{�U��1�X�e�[�g�ɓ��������̃��\�b�h
void AttackCombo1State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKesakiri, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// �R���{�U��1�X�e�[�g�Ŏ��s���郁�\�b�h
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

// �R���{�U��1�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackCombo1State::Exit()
{

}

// �R���{�U��2�X�e�[�g�ɓ��������̃��\�b�h
void AttackCombo2State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// �R���{�U��2�X�e�[�g�Ŏ��s���郁�\�b�h
void AttackCombo2State::Execute(float elapsedTime)
{
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

// �R���{�U��2�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackCombo2State::Exit()
{

}

// �R���{�U��3�X�e�[�g�ɓ��������̃��\�b�h
void AttackCombo3State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, 2.0f);
}

// �R���{�U��3�X�e�[�g�Ŏ��s���郁�\�b�h
void AttackCombo3State::Execute(float elapsedTime)
{
	// �A�j���[�V�����Đ����I�������
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

// �R���{�U��3�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackCombo3State::Exit()
{

}

// �_�b�V���U���X�e�[�g�ɓ��������̃��\�b�h
void AttackDashuState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false);
	stateTimer = 0.0f;
	nextAttackFlag = false;
}

// �_�b�V���U���X�e�[�g�Ŏ��s���郁�\�b�h
void AttackDashuState::Execute(float elapsedTime)
{
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
			// 1�b�o������ҋ@�X�e�[�g�ֈړ�
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Action);
			stateTimer += elapsedTime;
		}
	}
}

// �_�b�V���U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackDashuState::Exit()
{

}

// ����X�e�[�g�ɓ��������̃��\�b�h
void AvoiDanceState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Block, false);
}

// ����X�e�[�g�Ŏ��s���郁�\�b�h
void AvoiDanceState::Execute(float elapsedTime)
{
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Action);
	}
}

// ����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AvoiDanceState::Exit()
{

}