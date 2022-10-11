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


};