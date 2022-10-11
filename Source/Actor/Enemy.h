#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Character.h"
#include "PlayerManager.h"

class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy()override {}

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive() {};

    virtual void DrawDebugGUI() {};

    //破棄
    void Destroy();

    // ゲッター　セッター
    // プレイヤーの位置取得
    DirectX::XMFLOAT3 GetPlayerPosition()
    {
        return PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
    }

    // 実行タイマー設定
    void SetRunTimer(float timer) { runTimer = timer; }

    // 実行タイマー取得
    float GetRunTimer() { return runTimer; }

    // プレイヤー索敵
    bool SearchPlayer();

    // ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    // 攻撃範囲取得
    float GetAttackRange() { return attackRange; }

private:
    DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
    float territoryRange = 10.0f;
    float searchRange = 5.0f;
    float attackRange = 1.5f;
    float runTimer = 0.0f;
};