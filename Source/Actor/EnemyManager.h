#pragma once

#include <vector>
#include "Enemy.h"

//エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

    void CollisionEnemyVsEnemies();

public:
    //唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    //エネミー全開放
    void Clear();

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //エネミー登録
    void Register(Enemy* enemy);

    void DrawDebugPrimitive();

    void DrawDebugGUI();

    // ゲッター  セッター
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    Enemy* GetEnemy(int index) const { return enemies.at(index); }

    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;
};