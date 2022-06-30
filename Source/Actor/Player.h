#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Character.h"

// �v���C���[
class Player : public Character
{
private:
    enum class Animation
    {

    };

public:
    Player();
    ~Player() override;

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

private:
    void InputMove(float elapsedTime);

    void InputAttack(float elapedTime);

    void CollisionPlayerVsEnemies();

    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    // �Q�b�^�[�@�Z�b�^�[
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    Model* model = nullptr;

    const float size = 0.015f;
    const float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);
};