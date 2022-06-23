#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}