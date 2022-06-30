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

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�G�l�~�[�o�^
    void Register(Enemy* enemy);

    void DrawDebugPrimitive();

    void DrawDebugGUI();

    // �Q�b�^�[  �Z�b�^�[
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    Enemy* GetEnemy(int index) const { return enemies.at(index); }

    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;
};