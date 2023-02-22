#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>

class Effect
{
public:
    Effect(const char* filename);
    ~Effect();
    // �Đ�
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);
    // ��~
    void Stop(Effekseer::Handle handle);
    // ���W�ݒ�
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);
    // �p�x�ݒ�
     void SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& rotation);
    // �X�P�[���ݒ�
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);
    // �F�ݒ�
    void SetColor(Effekseer::Handle handle, const DirectX::XMFLOAT4& color);
private:
    Effekseer::Effect* effekseerEffect = nullptr;
};
