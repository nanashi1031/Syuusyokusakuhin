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
    // �������͏����X�V
    void UpdateVerticalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);
    // �������͏����X�V
    void UpdateHorizontalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

    // �_�����𒎂ƃJ�����Ƃ̊Ԃֈړ�����
    void UpdateLight();

    // �������Ă���G�L�X�̐F�ɂ���ďo���G�t�F�N�g���Ǘ�����
    void UpdateExtractEffect();

private:
    std::unique_ptr<Effect> ParticleRed = nullptr;
    std::unique_ptr<Effect> ParticleWhite = nullptr;
};