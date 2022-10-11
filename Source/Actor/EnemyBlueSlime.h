#pragma once
#include "Graphics/Model.h"
#include "EnemySlime.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;

class EnemyBlueSlime :public Enemy
{
public:
	EnemyBlueSlime();
	~EnemyBlueSlime()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();

	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

	// �v���C���[���G
	bool SearchPlayer();

	// �^�[�Q�b�g�|�W�V�����ݒ�
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// �^�[�Q�b�g�|�W�V�����擾
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// �|�W�V�����擾
	DirectX::XMFLOAT3 GetPosition() { return position; }

	// �U���͈͎擾
	float GetAttackRange() { return attackRange; }

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI();

	// ���s�^�C�}�[�ݒ�
	void SetRunTimer(float timer) { runTimer = timer; }

	// ���s�^�C�}�[�擾
	float GetRunTimer() { return runTimer; }

	Model* GetModel() { return model; }

	void SetHealth(int setHealth) { health = setHealth; }
protected:
	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;
public:
	// �A�j���[�V����
	enum class EnemyAnimation
	{
		IdleNormal,
		IdleBattle,
		Attack01,
		Attack02,
		WalkFWD,
		WalkBWD,
		WalkLeft,
		WalkRight,
		RunFWD,
		SenseSomethingST,
		SenseSomethingRPT,
		Taunt,
		Victory,
		GetHit,
		Dizzy,
		Die,
	};


private:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				moveSpeed = 3.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				searchRange = 5.0f;
	float				attackRange = 1.5f;
	float				runTimer = 0.0f;

	BehaviorTree*		aiTree = nullptr;
	BehaviorData*		behaviorData = nullptr;
	NodeBase*			activeNode = nullptr;
};