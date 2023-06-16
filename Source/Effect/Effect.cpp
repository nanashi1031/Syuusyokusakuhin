#include "Graphics/Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

Effect::Effect(const char* filename)
{
    // Effekseer�̃��\�[�X��ǂݍ���
    // Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);
    // Effekseer::Manager���擾
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    // Effekseer�G�t�F�N�g��ǂݍ���
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}

Effect::~Effect()
{
    // �j������
    if (effekseerEffect != nullptr)
    {
        effekseerEffect->Release();
        effekseerEffect = nullptr;
    }
}

Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y,
        position.z);
    effekseerManager->SetScale(handle, scale, scale, scale);
    return handle;
}

void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->StopEffect(handle);
}

void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

void Effect::SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& rotation)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->SetRotation(handle, rotation.x, rotation.y, rotation.z);
}

void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::SetColor(Effekseer::Handle handle, const DirectX::XMFLOAT4& color)
{
    Effekseer::Manager* effekseerManager = EffectManager::Instance().GetEffekseerManager();
    Effekseer::Color colors;
    colors.R = static_cast<uint8_t>(color.x);
    colors.G = static_cast<uint8_t>(color.y);
    colors.B = static_cast<uint8_t>(color.z);
    colors.A = static_cast<uint8_t>(color.w);
    effekseerManager->SetAllColor(handle, colors);
}