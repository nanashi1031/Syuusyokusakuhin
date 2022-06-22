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

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //エネミー登録
    void Register(Enemy* enemy);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //デバッグ用GUI描画
    void DrawDebugGUI();

    //エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    //エネミー削除
    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;
};