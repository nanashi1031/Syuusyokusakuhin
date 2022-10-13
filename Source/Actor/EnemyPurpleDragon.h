#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;

class EnemyPurpleDragon : public Enemy
{
public:
	enum class EnemyPurpleDragonAnimation
	{
		IdleNormal,
		FryFound,
		AttackBiting,
		FryTakeOf,
		FryLanding,
		Scream,
		WingStrike,
		Flame,
		Defend,
		Wince,
		Sleep,
		WalkFound,
		RunFound,
		FryFrameAttack,
		FryGlide,
		IdelBattle,
		Die,
		FryFloat,
	};

public:
    EnemyPurpleDragon();
    ~EnemyPurpleDragon() override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// �^�[�Q�b�g�|�W�V�����ݒ�
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

private:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				searchRange = 5.0f;
};