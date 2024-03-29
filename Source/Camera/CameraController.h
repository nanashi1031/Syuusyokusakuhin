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
        int index; // 敵のインデックス番号
        float enemyLengthTotal; // 敵との距離

        // これをしないとソートできない
        // 最後のconstを忘れると"instantiated from here"というエラーが出てコンパイルできないので注意
        bool operator<(const Target& right) const
        {
            return right.enemyLengthTotal == right.enemyLengthTotal ? enemyLengthTotal > right.enemyLengthTotal : enemyLengthTotal > right.enemyLengthTotal;
        }
    };
    std::vector<Target> targets;

    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;  // 法線からの最短距離
    };

    enum class CameraMode
    {
        NormalCamera, // カメラ通常状態
        LockOnCamera, // ロックオンカメラ
        TransitionCamera, // 遷移カメラ
    };

public:
    static CameraController& Instance()
    {
        static CameraController cameraController;
        return cameraController;
    }

    CameraController() {}
    ~CameraController() {}

    void Update(float elapsedTime);
    void DrawDebugPrimitive();
    void DrawDebugGUI();

    // ゲッター　セッター
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    int GetTagetIndex() const { return this->nowTargetIndex; }

    void SetCamerarShake(bool flag,  DirectX::XMFLOAT3 power = { 0.0f, 0.0f, 0.0f });

    bool GetLockOnFlag() const { return this->lockOnFlag; }

    bool GetCameraMouseOperationFlag() const { return this->cameraMouseOperationFlag; }
    void SetCameraMouseOperationFlag(bool b) { this->cameraMouseOperationFlag = b; }

    std::vector<Target> GetTargets() { return this->targets; }

private:
    void UpdateMouse(float elapsedTime);
    void UpdatePad(float elapsedTime);

    void CameraRotationAxisLimit();

    void UpdateNormalCamera(float elapsedTime);
    void UpdateLockOnCamera(float elapsedTime);
    void UpdateTransitionCamera(float elapsedTime);

    void UpdateStageRayCast();

    void LockOn(float elapsedTime);
    bool LockOnSwitching();
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);

    void ShakeCamera(DirectX::XMFLOAT3 shakePower);

    bool GetTargetPerspective();
    DirectX::XMFLOAT3 GetPerspective();

    void CalculationEnemyLenght();

private:
    CameraMode cameraMode = CameraMode::NormalCamera;

    DirectX::XMFLOAT3 perspective = { 0, 0, 0 };
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(20), 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 8.0f;
    float lockOnCameraHeight = 4.0f;
    const float maxAngleX = DirectX::XMConvertToRadians(45);
    const float minAngleX = DirectX::XMConvertToRadians(-45);

    DirectX::XMFLOAT3 beforePerspective = { 0, 0, 0 };
    DirectX::XMFLOAT3 afterPerspective = { 0, 0, 0 };
    DirectX::XMFLOAT3 afterTarget = { 0, 0, 0 };
    DirectX::XMFLOAT3 targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : 座標, 1 : 注視点
    float lengthLimit[2] = { 5, 7 };
    float sideValue = 1;

    float lockOnRange = 5.0f;
    int nowTargetIndex = -1;
    bool lockOnFlag = false;
    float lockOnPositionY = 5.0f;
    float lockOnTimer = 0;
    bool cameraMouseOperationFlag = false;
    float interpolationPosition;

    int	collisionState = 0;
    float nearCamera = 0.0f;
    float farCamera = 0.0f;
    float lerpTimer = 0.0f;
    // 線形補間速度
    float lerpSpeed = 4.0f;

    bool lerpFlag = false;

    DirectX::XMFLOAT3 shakePower = { 1.0f, 1.0f, 1.0f };
    float shakesuppress = 0.01f;
    float shakeTimer = 0.0f;
    bool shakeFlag = true;
};