#pragma once

#include <DirectXMath.h>

// カメラコントローラー
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    void Update(float elapsedTime);
    void DrawDebugGUI();

    DirectX::XMFLOAT3 LockOn(float elapsedTime);
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);

    DirectX::XMFLOAT3 GetPerspective();

    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    int GetTagetIndex() const { return this->targetIndex; }

private:
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float playerRange = 10.0f;

    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);

    int targetIndex = 0;
    bool lockOnFlag = false;
    float lockOnPossitionY = 0.0f;
    float lockOnTimer = 0;
};