#pragma once

#include "Shader.h"
#include "Model.h"
#include "Character.h"

// プレイヤー
class Player : public Character
{
public:
    enum class State
    {
        Idle,
        Neglect,
        Walk,
        Run,
        AttackCombo1,
        AttackCombo2,
        AttackCombo3,
        AttackDashu,
        Avoidance,
        Damages,
        Die,
    };

    enum class PlayerAnimation
    {
        AnimNon,
        SwordStorage,
        SwordDrawing,
        TurnRun,
        TurnWalk,
        SlashRotaryLeft,
        SlashRotary,
        HiltPunch,
        SlashKillingBlow,
        BlockBack,
        Block,
        BlockStart,
        SkillMagic,
        SkillSword,
        CrouchBlockHit,
        CrouchBlockIdle,
        CrouchBlockStart,
        CrouchIdle,
        Crouch,
        CrouchHitSmall,
        CrouchhitBig,
        CrouchEnd,
        DeathFront,
        DeathBack,
        Neglect1,
        Neglect2,
        Idle,
        Neglect3,
        HitSmall,
        HitBig,
        HitBlock,
        Jump,
        JumpFront,
        Kick,
        PowerUp,
        RunBack,
        RunFront,
        SlashTripleAttack,
        SlashLeftRoundUp,
        SlashKaratake,
        SlashLeg,
        SlashKesakiri,
        StrafeLeft,
        StrafeRunLeft,
        StrafeRunRight,
        StrafeRight,
        TurnLeft,
        TurnRight,
        WalkBack,
        WalkFront,
        Avoidance,
    };

public:
    Player();
    ~Player() override;

    void Update(float elapsedTime);
    void Render(RenderContext rc, ModelShader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    // ゲッター　セッター
    float GetDefensePower() { return defensePower; }

protected:
     // ダメージを受けたときに呼ばれる
     void OnDamaged() override;

     // 死亡したときに呼ばれる
     void OnDead() override;

private:
    void InputMove(float elapsedTime);

    void CollisionPlayerVsEnemies();

    DirectX::XMFLOAT3 GetMoveVec();

    void HealthMax()
    {
        if (health > maxHealth)
            health = maxHealth;
    }

private:
    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float defensePower = 1.0f;
};