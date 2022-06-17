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
    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

private:
    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾
    //DirectX::XMFLOAT3 GetMoveVec();
    void Move();

private:
    Model* model = nullptr;

    const float k_size = 0.015f;
    const float moveSpeed = 0.1f;
};