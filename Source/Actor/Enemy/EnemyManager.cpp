#include "EnemyManager.h"
#include "Collision.h"

//�G�l�~�[�S�폜
void EnemyManager::Clear()
{
    enemies.clear();
}

//�X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    CollisionEnemyVsEnemies();

    //�j������
    //��projectiles�͈̔�for���̒���eraser()����ƕs����������Ă��܂����߁A
    //  �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Enemy* enemy : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(
            enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        //�e�ۂ̔j������
        delete enemy;
    }
    //�j�����X�g���N���A
    removes.clear();
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = enemyManager.GetEnemy(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsSpherer(
                enemyA->GetPosition(), enemyA->GetRadius(),
                enemyB->GetPosition(), enemyB->GetRadius(),
                outPosition))
            {
                //�����o����̈ʒu�ݒ�
                enemyB->SetPosition(outPosition);
            }
        }
    }
}

//�`�揈��
void EnemyManager::Render(RenderContext rc, ModelShader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(rc, shader);
    }
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}

//GUI�`��
void EnemyManager::DrawDebugGUI()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugGUI();
    }
}


void EnemyManager::Remove(Enemy* enemy)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(enemy);
}