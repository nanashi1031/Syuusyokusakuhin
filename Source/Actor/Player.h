#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Character.h"

// プレイヤー
class Player : public Character
{
public:
    enum class State
    {
        Action,
        Battle,
        Dash,
        Avoid,
        Damage,
        Death,
    };

    enum class Action
    {
        Idle,
        Neglect,
        Walk,
        Run,
    };

    enum class Battle
    {
        AttackCombo1,
        AttackCombo2,
        AttackCombo3,
    };

    enum class Dash
    {
        AttackDashu,
    };

    enum class Avoid
    {
        Avoidance,
    };

    enum class Damage
    {
        Damages,
    };

    enum class Death
    {
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
    };

public:
    Player();
    ~Player() override;

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    // ゲッター　セッター
    // 前方向を取得
    DirectX::XMFLOAT3 GetFront() const;

protected:
     // ダメージを受けたときに呼ばれる
     void OnDamaged() override;

     // 死亡したときに呼ばれる
     void OnDead() override;

private:
    void InputMove(float elapsedTime);

    void InputAttack(float elapedTime);

    void CollisionPlayerVsEnemies();

    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    DirectX::XMFLOAT3 GetMoveVec();

public:
    // プレイヤーのノードの名前定数
    const char* swordName = "mixamorig:Sword_joint";

private:
    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);
};