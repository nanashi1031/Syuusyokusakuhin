#pragma once
#include "Effect.h"

class Insect : public Character
{
public:
    enum class State
    {
        Idle,
        Pursuit,
        Flying,
        Return,
    };

public:
    Insect();
    ~Insect();

    void Initialize();
    void Update(float elapsedTime);
    void Render(RenderContext rc, ModelShader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

private:
    // 垂直速力処理更新
    void UpdateVerticalVelocity(float elapsedFrame);
    // 垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);
    // 水平速力処理更新
    void UpdateHorizontalVelocity(float elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

    // 点光源を虫とカメラとの間へ移動する
    void UpdateLight();

    // 所持しているエキスの色によって出すエフェクトを管理する
    void UpdateExtractEffect();

private:
    std::unique_ptr<Effect> ParticleRed = nullptr;
    std::unique_ptr<Effect> ParticleWhite = nullptr;
};