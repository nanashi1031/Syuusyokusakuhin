#include "Input.h"
#include "PlayerDerived.h"
#include "Mathf.h"
#include "Collision.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "LightManager.h"
#include "Camera.h"
#include "CameraController.h"
#include "SceneManager.h"
#include "SceneGame.h"

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
	// �X�e�B�b�Nor�L�[�{�[�h�ł̈ړ���10�b�ȏ㖳���ꍇ���u�X�e�[�g�ֈړ�
	else if(owner->GetMoveFlag() == 0.0f)
	{
		float animationEndTime = 10.0f;
		if (stateTimer > 5.0f)
		{
			owner->GetStateMachine()->ChangeState(Player::State::Neglect);
		}
		stateTimer += elapsedTime;
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}
}

void PlayerState::IdleState::Exit()
{

}

void PlayerState::NeglectState::Enter()
{
	NeglectVoice0 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/NeglectVoice0.wav");
	NeglectVoice1 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/NeglectVoice1.wav");
	NeglectVoice2 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/NeglectVoice2.wav");
	float volume = 0.4f;

	// �����_���ŕ��u�A�j���[�V�������Đ�
	int neglectIndex = static_cast<int>(Mathf::RandomRange(0, 3));
	switch (neglectIndex)
	{
	case 0:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect1, false);
		NeglectVoice0->Play(false, volume);
		break;
	case 1:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect2, false);
		NeglectVoice1->Play(false, volume);
		break;
	case 2:
		owner->GetModel()->PlayAnimation(Player::PlayerAnimation::Neglect3, false);
		NeglectVoice2->Play(false, volume);
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

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
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

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	if ((gamePad.GetButtonDown() & gamePad.BTN_SPACE) ||
		(gamePad.GetButtonDown() & gamePad.BTN_B))
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
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

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		owner->GetStateMachine()->ChangeState(Player::State::AttackCombo1);
	}

	if ((gamePad.GetButtonDown() & gamePad.BTN_SPACE) ||
		(gamePad.GetButtonDown() & gamePad.BTN_B))
		owner->GetStateMachine()->ChangeState(Player::State::Avoidance);
}

void PlayerState::RunState::Exit()
{

}

void PlayerState::AttackCombo1State::Enter()
{
	SE_Attack1 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/Attack.wav");
	hitEffect1 = new Effect("Data/Effect/Hit.efk");
	float animeSpeed = 2.0f;
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKesakiri, false, 1.2f, animeSpeed);
	stateTimer = 0.0f;
	nextAttackFlag = false;
	owner->SetMovingFlag(false);
	DirectX::XMFLOAT3 vec = PlayerState::Versatility::Direction();
	PlayerState::Versatility::Rotate(vec);
}

void PlayerState::AttackCombo1State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// �A�j���[�V�����Đ����I����
	float nextAnimeSeconds = 0.9f;
	if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
	{
		// �U���t���O��true�Ȃ�
		if (nextAttackFlag)
		{
			owner->GetStateMachine()->ChangeState(Player::State::AttackCombo2);
		}
		else if (!nextAttackFlag)
		{
			// 0�b�o������ҋ@�X�e�[�g�ֈړ�
			float animationEndTime = 0.5f;
			if (stateTimer >= animationEndTime)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
	else
	{
		for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
		{
			Enemy* enemy = enemyManager.GetEnemy(i);
			for (int j = 0; j < enemy->GetParts().size(); j++)
			{
				float radius = 1.0f;
				float attackPower =
					Mathf::PlayerAttackDamageCalculation(
						5.0f, enemy->GetParts()[j].defensePower);
				DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
				float invincibleTime = 0.5f;
				if (Collision::AttackNodeVsNode(
					owner, "mixamorig:Sword_joint", radius,
					enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
					hitPosition, attackPower, invincibleTime))
				{
					if (owner->GetLightIndex() < 0)
					{
						DirectX::XMFLOAT4 color =
							Extract::Instance().ColorConversion(enemy->GetParts()[j].extractColor);
						Light* light = new Light(LightType::Point);
						DirectX::XMFLOAT3 lightPosition =
							owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
						light->SetPosition(lightPosition);
						light->SetColor(DirectX::XMFLOAT4({ color }));
						light->SetRange(2.0f);
						LightManager::Instane().Register(light);
						owner->SetLightIndex(LightManager::Instane().GetLightCount());
						hitEffect1->Play(lightPosition);
					}
					float volume = 0.5f;
					SE_Attack1->Play(false, volume);
				}
			}
		}
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		nextAttackFlag = true;
	}
}

void PlayerState::AttackCombo1State::Exit()
{
	delete hitEffect1;

	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackCombo2State::Enter()
{
	SE_Attack2 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/Attack.wav");
	hitEffect2 = new Effect("Data/Effect/Hit.efk");

	float animeSpeed = 2.0f;
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashLeftRoundUp, false, 1.2f, animeSpeed);
	stateTimer = 0.0f;
	nextAttackFlag = false;
	owner->SetMovingFlag(false);
	DirectX::XMFLOAT3 vec = PlayerState::Versatility::Direction();
	PlayerState::Versatility::Rotate(vec);
}

void PlayerState::AttackCombo2State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// �A�j���[�V�����Đ����I����
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
			float animationEndTime = 0.5f;
			if (stateTimer >= animationEndTime)
				owner->GetStateMachine()->ChangeState(Player::State::Idle);
			stateTimer += elapsedTime;
		}
	}
	else
	{
		nextAnimeSeconds = 0.4f;
		if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSeconds)
		{
			for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
			{
				Enemy* enemy = enemyManager.GetEnemy(i);
				for (int j = 0; j < enemy->GetParts().size(); j++)
				{
					float radius = 1.0f;
					float attackPower =
						Mathf::PlayerAttackDamageCalculation(5.0f, enemy->GetParts()[j].defensePower);
					DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
					float invincibleTime = 0.3f;
					if (Collision::AttackNodeVsNode(
						owner, "mixamorig:Sword_joint", radius,
						enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
						hitPosition, attackPower, invincibleTime))
					{
						if (owner->GetLightIndex() < 0)
						{
							DirectX::XMFLOAT4 color =
								Extract::Instance().ColorConversion(
									enemy->GetParts()[j].extractColor);
							Light* light = new Light(LightType::Point);
							DirectX::XMFLOAT3 lightPosition =
								owner->GetNodePosition(
									owner->GetNode("mixamorig:Sword_joint"));
							light->SetPosition(lightPosition);
							light->SetColor(DirectX::XMFLOAT4({ color }));
							light->SetRange(2.0f);
							LightManager::Instane().Register(light);
							owner->SetLightIndex(
								LightManager::Instane().GetLightCount());
							hitEffect2->Play(lightPosition);
							owner->SetLightIndex(
								LightManager::Instane().GetLightCount());
							DirectX::XMFLOAT3 rotation =
							{ 0, 0, DirectX::XMConvertToRadians(180) };
							int effectIndex = hitEffect2->Play(lightPosition);
							hitEffect2->SetRotation(effectIndex, rotation);
						}
						float volume = 0.5f;
						SE_Attack2->Play(false, volume);
					}
				}
			}
		}
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
	{
		nextAttackFlag = true;
	}
}

void PlayerState::AttackCombo2State::Exit()
{
	delete hitEffect2;

	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackCombo3State::Enter()
{
	SE_Attack3 = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/Attack.wav");
	hitEffect3 = new Effect("Data/Effect/Hit.efk");

	float animeSpeed = 2.0f;
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashKaratake, false, 1.2f, animeSpeed);
	owner->SetMovingFlag(false);
	DirectX::XMFLOAT3 vec = PlayerState::Versatility::Direction();
	PlayerState::Versatility::Rotate(vec);
}

void PlayerState::AttackCombo3State::Execute(float elapsedTime)
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	float nextAnimeSecondsStart = 1.0f;
	float nextAnimeSecondsStEnd = 2.0f;
	if (owner->GetModel()->GetAnimationSeconds() > nextAnimeSecondsStart &&
		owner->GetModel()->GetAnimationSeconds() < nextAnimeSecondsStEnd)
	{
		for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
		{
			Enemy* enemy = enemyManager.GetEnemy(i);
			for (int j = 0; j < enemy->GetParts().size(); j++)
			{
				float attackPower =
					Mathf::PlayerAttackDamageCalculation(
						10.0f, enemy->GetParts()[j].defensePower);
				float radius = 1.0f;
				DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
				float invincibleTime = 1.0f;
				if (Collision::AttackNodeVsNode(
					owner, "mixamorig:Sword_joint", radius,
					enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
					hitPosition, attackPower, invincibleTime))
				{
					if (owner->GetLightIndex() < 0)
					{
						DirectX::XMFLOAT4 color =
							Extract::Instance().ColorConversion(enemy->GetParts()[j].extractColor);
						Light* light = new Light(LightType::Point);
						DirectX::XMFLOAT3 lightPosition =
							owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
						light->SetPosition(lightPosition);
						light->SetColor(DirectX::XMFLOAT4({ color }));
						light->SetRange(2.0f);
						LightManager::Instane().Register(light);
						owner->SetLightIndex(LightManager::Instane().GetLightCount());
						DirectX::XMFLOAT3 rotation = {0, 0, DirectX::XMConvertToRadians(45) };
						int effectIndex = hitEffect3->Play(lightPosition);
						hitEffect3->SetRotation(effectIndex, rotation);
					}
					float volume = 0.5f;
					SE_Attack3->Play(false, volume);
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
	delete hitEffect3;

	if (owner->GetLightIndex() > 0)
	{
		LightManager::Instane().RemoveIndex(owner->GetLightIndex());
		owner->SetLightIndex(-1);
	}

	owner->SetMovingFlag(true);
}

void PlayerState::AttackDashuState::Enter()
{
	float animeSpeed = 2.0f;
	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::SlashRotary, false, 0.0f, animeSpeed);
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
			float radius = 1.0f;
			float attackPower =
				Mathf::PlayerAttackDamageCalculation(10.0f, enemy->GetParts()[j].defensePower);
			DirectX::XMFLOAT3 hitPosition = { 0.0f, 0.0f, 0.0f };
			if (Collision::AttackNodeVsNode(
				owner, "mixamorig:Sword_joint", radius,
				enemy, enemy->GetParts()[j].name, enemy->GetParts()[j].radius,
				hitPosition, attackPower))
			{
				if (owner->GetLightIndex() < 0)
				{
					DirectX::XMFLOAT4 color =
						Extract::Instance().ColorConversion(enemy->GetParts()[j].extractColor);
					Light* light = new Light(LightType::Point);
					DirectX::XMFLOAT3 lightPosition =
						owner->GetNodePosition(owner->GetNode("mixamorig:Sword_joint"));
					light->SetPosition(lightPosition);
					light->SetColor(DirectX::XMFLOAT4({ color }));
					light->SetRange(2.0f);
					LightManager::Instane().Register(light);
					owner->SetLightIndex(LightManager::Instane().GetLightCount());
				}
			}
		}
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	if ((mouse.GetButtonDown() & mouse.BTN_LEFT) ||
		(gamePad.GetButtonDown() & gamePad.BTN_A))
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
	float animationTime = 0.0f;

	if (Extract::Instance().GetExtract(ExtractColor::White) > 0.0f)
	{
		animationTime = 1.7f;
	}
	else
	{
		animationTime = 1.2f;
	}

	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::Avoidance, true, 0.2f, animationTime);
	owner->SetInvincibleTimer(animationTime);
	owner->SetMovingFlag(false);
	//DirectX::XMFLOAT3 directionFront = owner->GetFront();
	DirectX::XMFLOAT3 directionFront = PlayerState::Versatility::Direction();
	PlayerState::Versatility::Rotate(directionFront);
	directionFront.x *= 100.0;
	directionFront.z *= 100.0;
	owner->SetTargetPosition(directionFront);

	stateTimer = 0.0f;
}

void PlayerState::AvoiDanceState::Execute(float elapsedTime)
{
	Player* player = PlayerManager::Instance().GetPlayer(0);
	float animationEndTime = 0.0f;

	owner->MoveToTarget(elapsedTime, 30.0f);

	if (Extract::Instance().GetExtract(ExtractColor::White) > 0.0f)
	{
		animationEndTime = 0.9f;
	}
	else
	{
		animationEndTime = 1.2f;
	}

	if (stateTimer > animationEndTime)
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}

	stateTimer += elapsedTime;
}

void PlayerState::AvoiDanceState::Exit()
{
	stateTimer = 0.0f;
	owner->SetInvincibleTimer(0.0f);
	owner->SetMovingFlag(true);
	owner->SetVerocity({ 0.0f, 0.0f, 0.0f });
}

void PlayerState::DamagesState::Enter()
{
	DamageVoice = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/DamageVoice.wav");
	float volume = 0.7f;
	DamageVoice->Play(false, volume);

	owner->SetMovingFlag(false);

	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::HitBig, false);
}

void PlayerState::DamagesState::Execute(float elapsedTime)
{
	DirectX::XMFLOAT3 shakePower = { 10.0f, 10.0f, 10.0f };
	CameraController::Instance().SetCamerarShake(true, shakePower);

	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::DamagesState::Exit()
{
	CameraController::Instance().SetCamerarShake(false);

	owner->SetMovingFlag(true);
}

void PlayerState::DieState::Enter()
{
	owner->SetMovingFlag(false);

	owner->GetModel()->PlayAnimation(
		Player::PlayerAnimation::DeathFront, false);
}

void PlayerState::DieState::Execute(float elapsedTime)
{
	// �A�j���[�V�����Đ����I����
	if (!owner->GetModel()->IsPlayAnimation())
	{
		SceneManager::Instance().SetSceneGameState(SceneGameState::GameOver);
		owner->GetStateMachine()->ChangeState(Player::State::Idle);
	}
}

void PlayerState::DieState::Exit()
{
	owner->SetMovingFlag(true);
}

DirectX::XMFLOAT3 PlayerState::Versatility::Direction()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}
	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}
	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

	vec.y = 0.0f;
	return vec;
}

void PlayerState::Versatility::Rotate(DirectX::XMFLOAT3 vec)
{
	PlayerManager& playerManager = PlayerManager::Instance();
	Player* player = playerManager.GetPlayer(playerManager.GetplayerOneIndex());

	//float speed = DirectX::XMConvertToRadians(720) * 0.0166666675;
	float speed = DirectX::XMConvertToRadians(720) * 0.1f;

	if (vec.x == 0.0f && vec.z == 0.0f)
		return;

	float v = static_cast<float>(sqrt(pow(vec.x, 2.0f) + pow(vec.z, 2.0f)));
	vec.x /= v;
	vec.z /= v;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(player->GetAngle().y);
	float frontZ = cosf(player->GetAngle().y);

	//���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (vec.x * frontZ) - (vec.z * frontX);

	//��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (vec.x * frontX) + (vec.z * frontZ);

	//���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	//�������قǂ�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;

	//2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	//���E������s�����Ƃɂ���č��E��]��I������
	DirectX::XMFLOAT3 angle = player->GetAngle();
	if (cross < 0.0f)
	{
		angle.y -= speed * rot;
		player->SetAngle(angle);
	}
	else
	{
		angle.y += speed * rot;
		player->SetAngle(angle);
	}
}