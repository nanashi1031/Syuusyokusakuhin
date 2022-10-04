#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "EnemyBlueSlime.h"
#include "PLayerManager.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentDerived.h"
#include "ActionDerived.h"

// �R���X�g���N�^
EnemyBlueSlime::EnemyBlueSlime()
{
	model = new Model("Data/Model/BlueSlime/BlueSlime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.5f;
	height = 1.0f;

	maxHealth = 10.0f;

	// �r�w�C�r�A�c���[�ݒ�
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);
	//TODO 04_09 BehaviorTree�}�����BehaviorTree���\�z


	aiTree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Root", "Escape", 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Scout", 5, BehaviorTree::SelectRule::Priority, nullptr, nullptr);


	aiTree->AddNode("Scout", "Wander",1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	aiTree->AddNode("Scout", "Idle", 2, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));

	aiTree->AddNode("Escape", "Leave", 6, BehaviorTree::SelectRule::Non, nullptr, new LeaveAction(this));
	aiTree->AddNode("Escape", "Recover", 7, BehaviorTree::SelectRule::Non, nullptr, new RecoverAction(this));

	aiTree->AddNode("Battle", "Attack", 8, BehaviorTree::SelectRule::Random, new AttackJudgment(this), nullptr);
	aiTree->AddNode("Attack", "Normal", 9, BehaviorTree::SelectRule::Non, nullptr, new AttackAction(this));
	aiTree->AddNode("Attack", "Skill", 8, BehaviorTree::SelectRule::Non, new SkillShotJudgment(this), new SkillAction (this));
	aiTree->AddNode("Battle", "Pursuit", 10, BehaviorTree::SelectRule::Non, nullptr, new PursuitAction(this));

	SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, 25.0f));
	SetTerritory(GetPosition(), 10.0f);
	SetRandomTargetPosition();
}

// �f�X�g���N�^
EnemyBlueSlime::~EnemyBlueSlime()
{
	delete model;
	delete aiTree;
	delete behaviorData;
}

void EnemyBlueSlime::Update(float elapsedTime)
{
	// ���ݎ��s����Ă���m�[�h���������
	if (activeNode == nullptr)
	{
		// ���Ɏ��s����m�[�h�𐄘_����B
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	}
	// ���ݎ��s����m�[�h�������
	if (activeNode != nullptr)
	{
		// �r�w�C�r�A�c���[����m�[�h�����s�B
		activeNode = aiTree->Run(activeNode, behaviorData, elapsedTime);
	}

	// ���͏����X�V
	UpdateVelocity(elapsedTime);

	// �I�u�W�F�N�g�s��X�V
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void EnemyBlueSlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}


// ���S�������ɌĂ΂��
void EnemyBlueSlime::OnDead()
{
	Destroy();
}

void EnemyBlueSlime::DrawDebugPrimitive()
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
}

// �꒣��ݒ�
void EnemyBlueSlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}
// �^�[�Q�b�g�ʒu�������_���ݒ�
void EnemyBlueSlime::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// �ړI�n�_�ֈړ�
void EnemyBlueSlime::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(elapsedTime, vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}


bool EnemyBlueSlime::SearchPlayer()
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
void EnemyBlueSlime::DrawDebugGUI()
{
	std::string str = "";
	if(activeNode!=nullptr)
	{
		str = activeNode->GetName();
	}
	//�g�����X�t�H�[��
	if (ImGui::CollapsingHeader("EnemyBlueSlime", ImGuiTreeNodeFlags_DefaultOpen))
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

		ImGui::Text(u8"Behavior�@%s", str.c_str());
	}
}