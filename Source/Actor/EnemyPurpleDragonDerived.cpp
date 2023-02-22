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
	// �^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//TransitionPursuitState();
	}
	// �v���C���[���G
	Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
	if (enemy->SearchPlayer()) {
		// ����������ǐՃX�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Pursuit);
	}
}

void EnemyPurpleDragonState::IdleState::Exit()
{

}

void EnemyPurpleDragonState::PursuitState::Enter()
{
	//�^�C�}�[�����Z�b�g
	stateTimer = 10.0f;
	owner->GetModel()->PlayAnimation(EnemyPurpleDragon::EnemyPurpleDragonAnimation::RunFound, true);
}

void EnemyPurpleDragonState::PursuitState::Execute(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	owner->SetTargetPosition(PlayerManager::Instance().GetPlayer(0)->GetPosition());
	// �ڕW�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 5.0f);

	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	// �v���C���[�����G�͈͂���o���
	if (dist > 10.0f)
	{
		// �^�C�}�[���}�C�i�X
		stateTimer -= elapsedTime;
	}
	//�v���C���[�����G�͈͓��Ȃ�
	else
	{
		//�^�C�}�[�����Z�b�g
		stateTimer = 10.0f;
	}

	if (dist < 3.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::AttackCombo);
	}
	//�^�C�}�[��0�ɂȂ��
	if (stateTimer < 0.0f)
	{
		// �ҋ@�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}
}

void EnemyPurpleDragonState::PursuitState::Exit()
{

}

void EnemyPurpleDragonState::NeglectState::Enter()
{
	// �����_���ŕ��u�A�j���[�V�������Đ�
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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Walk);
	}

	// �A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�ֈړ�
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

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ������ĂȂ��ꍇ�ҋ@�X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeState(EnemyPurpleDragon::State::Idle);
	}

	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l�ȏ�Ȃ�
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

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l��菬�����Ȃ�
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

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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

	// �A�j���[�V�����Đ����I����
	//if (!owner->GetModel()->IsPlayAnimation())
	float nextAnimeSeconds = 0.9f;
	if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
	{
		// 0�b�o������ҋ@�X�e�[�g�ֈړ�
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
	// �A�j���[�V�����Đ����I����
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
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(0);
		enemy->Destroy();
	}
}

void EnemyPurpleDragonState::DieState::Exit()
{

}