#pragma once

#include <DirectXMath.h>
#include <vector>
#include <list>

// カメラコントローラー
class CameraController
{
private:
    struct Target
    {
        float enemyLengthTotal;     // 敵との距離
        int index;                  // 敵のインデックス番号

        // これをしないとソートできない
        // 最後のconstを忘れると"instantiated from here"というエラーが出てコンパイルできないので注意
        bool operator<(const Target& right) const
        {
            return right.enemyLengthTotal == right.enemyLengthTotal ? index < right.index : enemyLengthTotal < right.enemyLengthTotal;
        }
    };
    std::vector<Target> targets;

    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;   // 法線からの最短距離
    };

    enum class CameraContorollerState
    {
        NormalTargetState,  // カメラ通常状態
        LockOnTargetState,  // ロックオン
        TransitionState,    // 移動補間
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
    bool LockOnSwitching();
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);
    bool frustumCulling(DirectX::XMFLOAT3 position, float radius);

    void ShakeCamera(DirectX::XMFLOAT3 shakePower);

    bool GetTargetPerspective();
    DirectX::XMFLOAT3 GetPerspective();

private:
    CameraContorollerState state = CameraContorollerState::NormalTargetState;

    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 10.0f;

    const float maxAngleX = DirectX::XMConvertToRadians(45);
    const float minAngleX = DirectX::XMConvertToRadians(-45);

    DirectX::XMFLOAT3 perspective;
    DirectX::XMFLOAT3 perspectiveq;
    float lockOnRange = 0;
    int nowTargetIndex = -1;
    bool lockOnFlag = false;
    float lockOnPositionY = 5.0f;
    float lockOnTimer = 0;
    bool cameraMouseOperationFlag = false;
    float interpolationPosition;

    int	collisionState = 0;
    Plane				frustum[6] = {};
    DirectX::XMFLOAT3	nearPoint[4] = {};	// Nearの四角形の４頂点の座標
    DirectX::XMFLOAT3	farPoint[4] = {};	// Farの四角形の４頂点の座標
    float nearCamera = 0.0f;
    float farCamera = 0.0f;
    float lerpTimer = 0.0f;
    // 線形補間速度
    float lerpSpeed = 1.0f;

    // 線形補間フラグ
    bool lerpFlag;

    DirectX::XMFLOAT3 shakePower = {1.0f, 1.0f, 1.0f};
    float shakesuppress = 0;
};