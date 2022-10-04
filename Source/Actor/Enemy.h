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

    // �Q�b�^�[�@�Z�b�^�[
    DirectX::XMFLOAT3 GetPlayerPosition() { return PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition(); }

    //�j��
    void Destroy();
};