#include "Input\Input.h"
#include "PlayerDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "LightManager.h"

void PlayerState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Idle, true);
	stateTimer = 0;
}

void PlayerState::IdleState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ���10�ȏ㖳���ꍇ���u�X�e�[�g�ֈړ�
	else if(owner->GetMoveFlag() == 0.0f)
	{
		if (stateTimer > 10)
		{
			owner->GetStateMachine()->ChangeState(Player::State::Neglect);
		}
		stateTimer += elapsedTime;
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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

void PlayerState::NeglectState::Execute(float elapsedTime)
{
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ����������ꍇ�����X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() > 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}

	// �A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�ֈړ�
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ������ĂȂ��ꍇ�ҋ@�X�e�[�g�ֈړ�
	if (owner->GetMoveFlag() == 0.0f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}

	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l�ȏ�Ȃ�
	if (owner->GetMoveFlag() >= 0.5f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Run);
	}

	// ���N���b�N�����ꂽ��U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ��ʂ�����K��l��菬�����Ȃ�
	if (owner->GetMoveFlag() < 0.5f)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Walk);
	}

	// ���N���b�N�����ꂽ��_�b�V���U���X�e�[�g�֑J��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
		//owner->GetStateMachine()->ChangeState(Player::State::AttackDashu);
	}

	// �E�N���b�N�����ꂽ�����X�e�[�g�֑J��
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
	owner->SetMovingFlag(false);
}

void PlayerState::AttackCombo1State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		for (int j = 0; j < enemy->GetParts().size(); j++)
		{
			float attackPower =
				owner->DamageCalculation(5.0f, enemy->GetParts()[j].defensePower);
			if (Collision::AttackNodeVsNode(
				owner, "mixamorig:Sword_joint", 1.0f,
				enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
				attackPower))
			{
				if (owner->GetLightIndex() < 0)
				{
					DirectX::XMFLOAT4 color = Extract::ColorConversion(enemy->GetParts()[j].extractColor);
					Light* light = new Light(LightType::Point);
					DirectX::XMFLOAT3 LightPosition =
						owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
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

	// ���N���b�N������U���t���O������
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// �A�j���[�V�����Đ����I����
	//if (!owner->GetModel()->IsPlayAnimation())
	float nextAnimeSeconds = 0.9f;
	if(owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 0�b�o������ҋ@�X�e�[�g�ֈړ�
			if (stateTimer >= 0.0f)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackCombo1State::Exit()
{
	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackCombo2State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, 2.0f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
	owner->SetMovingFlag(false);
}

void PlayerState::AttackCombo2State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
		{
			Enemy* enemy = enemyManager.GetEnemy(i);
			for (int j = 0; j < enemy->GetParts().size(); j++)
			{
				float attackPower =
					owner->DamageCalculation(5.0f, enemy->GetParts()[j].defensePower);
				if(Collision::AttackNodeVsNode(
					owner, "mixamorig:Sword_joint", 1.0f,
					enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
					attackPower))
				{
					if (owner->GetLightIndex() < 0)
					{
						DirectX::XMFLOAT4 color = Extract::ColorConversion(enemy->GetParts()[j].extractColor);
						Light* light = new Light(LightType::Point);
						DirectX::XMFLOAT3 LightPosition =
							owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
						light->SetPosition(LightPosition);
						light->SetColor(DirectX::XMFLOAT4({ color }));
						light->SetRange(2.0f);
						LightManager::Instane().Register(light);
						owner->SetLightIndex(LightManager::Instane().GetLightCount());
					}
				}
			}
		}
	}

	// ���N���b�N������U���t���O������
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & mouse.BTN_LEFT)
	{
		nextAttackFlag = true;
	}

	// �A�j���[�V�����Đ����I����
	//if (!owner->GetModel()->IsPlayAnimation())
	float nextAnimeSeconds = 0.9f;
	if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo3);
		}
		else if (!nextAttackFlag)
		{
			// 1�b�o������ҋ@�X�e�[�g�ֈړ�
			if (stateTimer >= 1)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackCombo2State::Exit()
{
	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackCombo3State::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, 2.0f);
	owner->SetMovingFlag(false);
}

void PlayerState::AttackCombo3State::Execute(float elapsedTime)
{
	if (owner->GetModel()->GetAnimationSeconds() > 1.0f)
	{
		EnemyManager& enemyManager = EnemyManager::Instance();
		for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
		{
			Enemy* enemy = enemyManager.GetEnemy(i);
			for (int j = 0; j < enemy->GetParts().size(); j++)
			{
				float attackPower =
					owner->DamageCalculation(10.0f, enemy->GetParts()[j].defensePower);
				if (Collision::AttackNodeVsNode(
					owner, "mixamorig:Sword_joint", 1.0f,
					enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
					attackPower))
				{
					if (owner->GetLightIndex() < 0)
					{
						DirectX::XMFLOAT4 color = Extract::ColorConversion(enemy->GetParts()[j].extractColor);
						Light* light = new Light(LightType::Point);
						DirectX::XMFLOAT3 LightPosition =
							owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
						light->SetPosition(LightPosition);
						light->SetColor(DirectX::XMFLOAT4({ color }));
						light->SetRange(2.0f);
						LightManager::Instane().Register(light);
						owner->SetLightIndex(LightManager::Instane().GetLightCount());
					}
				}
			}
		}
	}

	// �A�j���[�V�����Đ����I�������
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::AttackCombo3State::Exit()
{
	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackDashuState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false, 0.0f, 1.5f);
	stateTimer = 0.0f;
	nextAttackFlag = false;
	owner->SetMovingFlag(false);
}

void PlayerState::AttackDashuState::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		for (int j = 0; j < enemy->GetParts().size(); j++)
		{
			float attackPower =
				owner->DamageCalculation(10.0f, enemy->GetParts()[j].defensePower);
			if (Collision::AttackNodeVsNode(
				owner, "mixamorig:Sword_joint", 1.0f,
				enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
				attackPower))
			{
				if (owner->GetLightIndex() < 0)
				{
					DirectX::XMFLOAT4 color = Extract::ColorConversion(enemy->GetParts()[j].extractColor);
					Light* light = new Light(LightType::Point);
					DirectX::XMFLOAT3 LightPosition =
						owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
					light->SetPosition(LightPosition);
					light->SetColor(DirectX::XMFLOAT4({ color }));
					light->SetRange(2.0f);
					LightManager::Instane().Register(light);
					owner->SetLightIndex(LightManager::Instane().GetLightCount());
				}
			}
		}
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
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 1�b�o������ҋ@�X�e�[�g�ֈړ�
			owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
}

void PlayerState::AttackDashuState::Exit()
{
	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AvoiDanceState::Enter()
{
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Block, true);
}

void PlayerState::AvoiDanceState::Execute(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	// �}�E�X���E�N���b�N���Ă���Ԃ͉���A��������Action�X�e�[�g��
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
	// �A�j���[�V�����Đ����I����
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
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::DieState::Exit()
{

}