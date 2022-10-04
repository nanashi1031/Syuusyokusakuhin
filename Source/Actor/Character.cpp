#include "Character.h"
#include "Collision.h"
#include "StageManager.h"

void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0.0f && vz == 0.0f)
        return;

    //�i�s�x�N�g����P�ʃx�N�g����
    float v = sqrt(pow(vx, 2.0f) + pow(vz, 2.0f));
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
        if (stageManager.GetStage(static_cast<int>(StageManager::StageName::ExampleStage)));
        {
            // �n�ʂɐڒn���Ă���
            position.y = hit.collisionPosition.y;

            // �X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.collisionNormal.x * hit.collisionNormal.x + hit.collisionNormal.z * hit.collisionNormal.z);
            slopeRate = 1.0f - (hit.collisionNormal.y / (normalLengthXZ + hit.collisionNormal.y));

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