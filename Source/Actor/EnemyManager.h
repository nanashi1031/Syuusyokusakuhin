#pragma once

#include <vector>
#include "Enemy.h"

//�G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

    void CollisionEnemyVsEnemies();

public:
    //�B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    //�G�l�~�[�S�J��
    void Clear();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�G�l�~�[�o�^
    void Register(Enemy* enemy);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //�G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    //�G�l�~�[�폜
    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;
};