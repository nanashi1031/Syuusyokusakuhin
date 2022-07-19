#pragma once

#include <DirectXMath.h>
#include <list>

// カメラコントローラー
class CameraController
{
private:
    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;   // 法線からの最短距離
    };

    enum class CameraContorollerState
    {
        NormalTargetState,   //
        LockOnTargetState,
        TransitionState,
    };

public:
    CameraController() {}
    ~CameraController() {}

    void Update(float elapsedTime);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    void CalculateFrustum(Plane* frustum);

    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    int GetTagetIndex() const { return this->nowTargetIndex; }

private:
    void UpdateMouse(float elapsedTime);
    void UpdatePad(float elapsedTime);
    void UpdateKeyboard(float elapsedTime);

    void CameraRotationAxisLimit();

    DirectX::XMFLOAT3 UpdateTransitionState(float elapsedTime);

    void LockOn(float elapsedTime);
    auto LockOnSwitching();
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);
    bool frustumCulling(DirectX::XMFLOAT3 position, float radius);

    void ShakeCamera(DirectX::XMFLOAT3 shakePower);

    void GetTargetPerspective();
    DirectX::XMFLOAT3 GetPerspective();

private:
    CameraContorollerState state = CameraContorollerState::NormalTargetState;

    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 10.0f;

    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);

    DirectX::XMFLOAT3 perspective;
    float loclOnRange = 0;
    int nowTargetIndex = 0;
    bool lockOnFlag = false;
    float lockOnPossitionY = 5.0f;
    float lockOnTimer = 0;
    bool cameraMouseOperationFlag = false;
    std::list<int> targetIndex = {};
    float interpolationPosition;

    int	collisionState = 0;
    Plane				frustum[6] = {};
    DirectX::XMFLOAT3	nearPoint[4] = {};	// Nearの四角形の４頂点の座標
    DirectX::XMFLOAT3	farPoint[4] = {};	// Farの四角形の４頂点の座標
    float nearCamera = 0.0f;
    float farCamera = 0.0f;
    int timerer = 0.0f;

    DirectX::XMFLOAT3 shakePower = {1.0f, 1.0f, 1.0f};
    float shakesuppress = 0;
};