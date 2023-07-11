#include "EnemyManager.h"
#include "Collision.h"

//エネミー全削除
void EnemyManager::Clear()
{
    enemies.clear();
}

//更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    CollisionEnemyVsEnemies();

    //破棄処理
    //※projectilesの範囲for文の中でeraser()すると不具合が発生してしまうため、
    //  更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Enemy* enemy : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Enemy*>::iterator it = std::find(
            enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        //弾丸の破棄処理
        delete enemy;
    }
    //破棄リストをクリア
    removes.clear();
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = enemyManager.GetEnemy(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsSpherer(
                enemyA->GetPosition(), enemyA->GetRadius(),
                enemyB->GetPosition(), enemyB->GetRadius(),
                outPosition))
            {
                //押し出し後の位置設定
                enemyB->SetPosition(outPosition);
            }
        }
    }
}

//描画処理
void EnemyManager::Render(RenderContext rc, ModelShader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(rc, shader);
    }
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}

//GUI描画
void EnemyManager::DrawDebugGUI()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugGUI();
    }
}


void EnemyManager::Remove(Enemy* enemy)
{
    //破棄リストに追加
    removes.emplace_back(enemy);
}