#include "EnemySlime.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "PlayerManager.h"
#include "StateDerived.h"

// �R���X�g���N�^
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.5f;
	height = 1.0f;

	// TODO 03_06 StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
	stateMachine = new StateMachine();
	// �X�e�[�g�}�V����1�w�ڂ̃X�e�[�g��o�^(BattleState�����l�̕��@�Ŋe���ǉ����Ă�������)
	stateMachine->RegisterState(new SearchState(this));
	stateMachine->RegisterState(new BattleState(this));
	// �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^(WanderState�ȊO��2�w�ڂ̃X�e�[�g�����l�̕��@�Ŋe���ǉ����Ă��������B)
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Battle), new AttackState(this));
	// �X�e�[�g���Z�b�g
	stateMachine->SetState(static_cast<int>(State::Search));
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
	delete model;
}

void EnemySlime::Update(float elapsedTime)
{
	// TODO 02_05 switch���ɂ��FSM��StateMachine�N���X���g�p�������̂ɏ���������
	stateMachine->Update(elapsedTime);

	// �I�u�W�F�N�g�s��X�V
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

// ���S�������ɌĂ΂��
void EnemySlime::OnDead()
{
	// ���S
	Destroy();
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}


void EnemySlime::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// �^�[�Q�b�g�ʒu���f�o�b�O���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// ���G�͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}

// �꒣��ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

// �^�[�Q�b�g�ʒu�������_���ݒ�
void EnemySlime::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemySlime::SearchPlayer()
{

	// �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
	PlayerManager& playerManager = PlayerManager::Instance();
	const DirectX::XMFLOAT3& playerPosition = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// �P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;

		// �����x�N�g����
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// �f�o�b�O�G�l�~�[���\��
void EnemySlime::DrawDebugGUI()
{

	// TODO 03_07 �f�o�b�O������\���̕ύX
	std::string str = "";
	std::string subStr = "";

	// TODO 05 �f�o�b�O�\���p�Ɋe�X�e�[�g����str�ɒǉ����Ȃ���
	// TODO 06�Q02 switch���ɂ��FSM��StateMachine�N���X���g�p�������̂ɏ���������
	switch (static_cast<State>(stateMachine->GetStateIndex())) {
	case State::Search:
		str = "Search";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Search::Wander))
		{
			subStr = "Wander";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Search::Idle))
		{
			subStr = "Idle";
		}
		break;
	case State::Battle:
		str = "Battle";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Battle::Pursuit))
		{
			subStr = "Pursuit";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Battle::Attack))
		{
			subStr = "Attack";
		}
		break;
	}


	//�g�����X�t�H�[��
	if (ImGui::CollapsingHeader("EnemySlime", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �ʒu
		ImGui::InputFloat3("Position", &position.x);
		// ��]
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// �X�P�[��
		ImGui::InputFloat3("Scale", &scale.x);

		ImGui::Text(u8"State�@%s", str.c_str());
		ImGui::Text(u8"SubState�@%s", subStr.c_str());
	}
}