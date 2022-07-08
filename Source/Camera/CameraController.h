#pragma once

#include <DirectXMath.h>

// カメラコントローラー
class CameraController
{
public:
    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;   // 法線からの最短距離
    };

public:
    CameraController() {}
    ~CameraController() {}

    void Update(float elapsedTime);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    void CalculateFrustum(Plane* frustum);

    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    int GetTagetIndex() const { return this->targetIndex; }

private:
    void UpdateMouse(float elapsedTime);
    void UpdatePad(float elapsedTime);
    void UpdateKeyboard(float elapsedTime);

    void CameraRotationAxisLimit();

    DirectX::XMFLOAT3 LockOn(float elapsedTime);
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);
    bool frustumCulling(DirectX::XMFLOAT3 position, float radius);

    DirectX::XMFLOAT3 GetPerspective();

private:
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 10.0f;

    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);

    int targetIndex = 0;
    bool lockOnFlag = false;
    float lockOnPossitionY = 5.0f;
    float lockOnTimer = 0;
    bool cameraMouseOperationFlag = false;

    int	collisionState = 0;
    Plane				frustum[6] = {};
    DirectX::XMFLOAT3	nearPoint[4] = {};	// Nearの四角形の４頂点の座標
    DirectX::XMFLOAT3	farPoint[4] = {};	// Farの四角形の４頂点の座標
    float nearCamera = 0.0f;
    float farCamera = 0.0f;
};