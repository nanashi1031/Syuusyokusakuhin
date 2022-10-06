#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

class EnemyPurpleDragon : public Enemy
{
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