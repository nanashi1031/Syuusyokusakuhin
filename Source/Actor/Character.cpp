#include "Character.h"
#include "Collision.h"
#include "StageManager.h"
#include "LightManager.h"

void Character::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;

	// �ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0.0f && vz == 0.0f)
        return;

    //�i�s�x�N�g����P�ʃx�N�g����
    float v = static_cast<float>(sqrt(pow(vx, 2.0f) + pow(vz, 2.0f)));
    vx /= v;
    vz /= v;

    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (vx * frontZ) - (vz * frontX);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (vx * frontX) + (vz * frontZ);

    //���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    //�������قǂ�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0f - dot;

    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        angle.y -= speed * rot;
    }
    else
    {
        angle.y += speed * rot;
    }

}

void Character::UpdateVelocity(float elapsedTime)
{
    // �o�߃t���[��
    float elapsedFrame = elapsedTime * 60.0f;

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    // �����ړ������X�V
    UpdateHorizontalMove(elapsedTime);
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    velocity.y += gravity * elapsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    float my = velocity.y * elapsedTime;
    slopeRate = 0.0f;

    // ������
    if (my < 0.0f)
    {
        // ���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset , position.z };
        // ���C�̏I���ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x,position.y + my , position.z };

        // ���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
		StageManager& stageManager = StageManager::Instance();
        if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
        {
            // �n�ʂɐڒn���Ă���
            position.y = hit.position.y;

            // �X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // ���n����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}

// �������͏����X�V
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsedFrame;

		// �󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!isGround) friction *= airControl;

		// ���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			// �P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// �������̑��x�����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsedFrame;
			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (!isGround)acceleration *= airControl;

			// �ړ��x�N�g���ɂ���������
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			// �ő呬�x����
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}
		}
	}
	// �ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	// �������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);

		// ���C�Ƃ̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx + DirectX::XMVectorGetX(Velocity), position.y + stepOffset, position.z + mz + DirectX::XMVectorGetZ(Velocity) };

		// ���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		StageManager& stageManager = StageManager::Instance();
		if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
		{
			// �ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			End = DirectX::XMVectorSubtract(End, Velocity);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// �ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			collectPosition.y = position.y;
			hit.position.y = position.y;

			// �ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!stageManager.GetStage(stageManager.GetNowStage())->RayCast(hit.position, collectPosition, hit2))
			{
				hit2.position.y = position.y;
				// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
#if 0
				position.x = collectPosition.x;
				position.z = collectPosition.z;
#endif
				hit.normal.y = 0;
				Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit.normal));
				Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit.position));
				Dot = DirectX::XMVector3Dot(Vec, Normal);
				DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&collectPosition), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
				float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&position)), Normal));

				// position���X�^�[�g�ɂ��邱�Ƃł��̏ꂩ�瓮���Ȃ�(�͂��������̂�)
				//start.y = position.y;
				//position = start;

				DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(Normal, radius + dot)));
			}
			else
			{
				//position.x = hit2.position.x;
				//position.z = hit2.position.z;

				//hit2.position.y = position.y;
				//hit2.normal.y = 0;
				//Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit2.normal));
				//Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit2.position));
				//Dot = DirectX::XMVector3Dot(Vec, Normal);
				//DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit2.position), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
			}
		}
		else
		{
			// �ړ�
			position.x += mx;
			position.z += mz;
		}

	}
}

void Character::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void Character::Ratate(DirectX::XMFLOAT3 rotateValue, float time)
{
	angle.x += rotateValue.x;
	angle.y += rotateValue.y;
	angle.z += rotateValue.z;
}

bool Character::ApplyDamage(const float damage, const float invincibleTime)
{
	// �_���[�W��0�����̏ꍇ�̓_���[�W����
	if (damage < 0) return false;

	// ���S���Ă���ꍇ�̓_���[�W����
	if (health <= 0) return false;

	// ���G(���)�̎��̓_���[�W����
	if (invincible) return false;

	// ���G���Ԓ��̓_���[�W����
	if (invincibleTimer > 0.0f) return false;

	health -= damage;

	// ���S�ʒm
	if (health <= 0)
	{
		health = 0;
		OnDead();
	}
	// �_���[�W�ʒm
	else
		OnDamaged();

	// �_���[�W���󂯂��̂Ŗ��G�ɂȂ�
	invincibleTimer = invincibleTime;

	return true;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���͂ɗ͂�^����
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

float Character::DamageCalculation(float attackPower, float defensePower)
{
	float outResult = attackPower - defensePower;
	if (outResult < 0.0f)
		outResult = 0.0f;

	return outResult;
}

void Character::UpdateInvincibleTime(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}