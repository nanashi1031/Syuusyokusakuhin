#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
//�f�o�b�O�pGUI�`��
void Enemy::DrawDebugGUI()
{

}

//�j��
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}