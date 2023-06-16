#include "Enemy.h"
#include "Graphics.h"
#include "EnemyManager.h"
#include "Mathf.h"

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

bool Enemy::SearchPlayer()
{

	// �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
	PlayerManager& playerManager = PlayerManager::Instance();
	const DirectX::XMFLOAT3& playerPosition =
		playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetPosition();
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

// �^�[�Q�b�g�ʒu�������_���ݒ�
void Enemy::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}