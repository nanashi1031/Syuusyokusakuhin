#pragma once
#include "Model.h"
#include "Enemy.h"
#include "Audio.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;

class EnemyPurpleDragon : public Enemy
{
public:
	enum class State
	{
		Idle,
		BattleIdle,
		Neglect,
		Howl,
		Pursuit,
		Walk,
		Run,
		TakeOff,
		Fly,
		Landing,
		Sleep,
		BiteAttack,
		ClawAttack,
		Damages,
		Die,
	};

	enum class Animation
	{
		IdleNormal,
		FryFound,
		AttackBiting,
		FryTakeOff,
		FryLanding,
		Howl,
		WingStrike,
		Flame,
		Defend,
		Wince,
		Sleep,
		WalkFound,
		RunFound,
		FryFrameAttack,
		FryGlide,
		BattleIdle,
		Die,
		FryFloat,
	};

	enum class PartGroup
	{
		Head,
		Body,
		Legs,
		Wings,
		Tail,
	};

public:
    EnemyPurpleDragon();
    ~EnemyPurpleDragon() override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(RenderContext rc, ModelShader* shader)override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	void DrawDebugGUI() override;

	// ���ʂ̏��ݒ�
	void PartsRegister();

	// �Z�b�^�[�Q�b�^�[
	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// �^�[�Q�b�g�|�W�V�����ݒ�
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

protected:
	// �_���[�W���󂯂��Ƃ��ɌĂ΂��
	void OnDamaged() override;

	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

	// �_�E�����ɌĂ΂��
	void OnDown() override;

private:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float				searchRange = 10.0f;

	std::unique_ptr<AudioSource> SE_Attack;
};