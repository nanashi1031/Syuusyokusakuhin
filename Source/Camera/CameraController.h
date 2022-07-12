#pragma once

#include <DirectXMath.h>
#include <list>

// �J�����R���g���[���[
class CameraController
{
public:
    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;   // �@������̍ŒZ����
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

    void LockOn(float elapsedTime);
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);
    bool frustumCulling(DirectX::XMFLOAT3 position, float radius);

    DirectX::XMFLOAT3 GetTargetPerspective();
    DirectX::XMFLOAT3 GetPerspective();

private:
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 10.0f;

    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(-45);

    float loclOnRange = 0;
    int nowTargetIndex = 0;
    bool lockOnFlag = false;
    float lockOnPossitionY = 5.0f;
    float lockOnTimer = 0;
    bool cameraMouseOperationFlag = false;
    std::list<float> targetIndex = {};

    int	collisionState = 0;
    Plane				frustum[6] = {};
    DirectX::XMFLOAT3	nearPoint[4] = {};	// Near�̎l�p�`�̂S���_�̍��W
    DirectX::XMFLOAT3	farPoint[4] = {};	// Far�̎l�p�`�̂S���_�̍��W
    float nearCamera = 0.0f;
    float farCamera = 0.0f;
};