#pragma once
#include <DirectXMath.h>

// オブジェクト
class Object
{
public:
    Object() {};
    virtual ~Object() {};

    virtual void UpdateTransform();

    // ゲッターセッター
    const DirectX::XMFLOAT3& GetPosition() const { return this->position; }
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    const DirectX::XMFLOAT3& GetAngle() const { return this->angle; }
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    const DirectX::XMFLOAT3& GetScale() const { return this->scale; }
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale; }

protected:
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};