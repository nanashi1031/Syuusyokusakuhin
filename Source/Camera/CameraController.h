#pragma once

#include <DirectXMath.h>
#include <vector>
#include <list>

// �J�����R���g���[���[
class CameraController
{
private:
    struct Target
    {
        float enemyLengthTotal;     // �G�Ƃ̋���
        int index;                  // �G�̃C���f�b�N�X�ԍ�
        DirectX::XMFLOAT3 position;

        // ��������Ȃ��ƃ\�[�g�ł��Ȃ�
        // �Ō��const��Y����"instantiated from here"�Ƃ����G���[���o�ăR���p�C���ł��Ȃ��̂Œ���
        bool operator<(const Target& right) const
        {
            return right.enemyLengthTotal == right.enemyLengthTotal ? index < right.index : enemyLengthTotal < right.enemyLengthTotal;
        }
    };
    std::vector<Target> targets;

    struct Plane
    {
        DirectX::XMFLOAT3 normal;
        float distance;   // �@������̍ŒZ����
    };

    enum class CameraMode
    {
        NormalCamera,  // �J�����ʏ���
        LockOnCamera,  // ���b�N�I���J����
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

    // �Q�b�^�[�@�Z�b�^�[
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    int GetTagetIndex() const { return this->nowTargetIndex; }

    void SetCamerarShake(DirectX::XMFLOAT3 shakePower, float shakeTime);

    bool GetLockOnFlag() const { return this->lockOnFlag; }

    std::vector<Target> GetTargets() { return this->targets; }

private:
    void UpdateMouse(float elapsedTime);
    void UpdatePad(float elapsedTime);
    void UpdateKeyboard(float elapsedTime);

    void CameraRotationAxisLimit();

    void UpdateNormalCamera(float elapsedTime);
    void UpdateLockOnCamera(float elapsedTime);

    DirectX::XMFLOAT3 UpdateTransitionState(float elapsedTime);

    void LockOn(float elapsedTime);
    bool LockOnSwitching();
    DirectX::XMFLOAT3 ResetCamera(float elapsedTime);

    void ShakeCamera(DirectX::XMFLOAT3 shakePower);

    bool GetTargetPerspective();
    DirectX::XMFLOAT3 GetPerspective();

private:
    CameraMode cameraMode = CameraMode::NormalCamera;

    DirectX::XMFLOAT3 perspective = { 0, 0, 0 };
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(20), 0, 0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float mouseRollSpeed = 0.11f;
    float playerRange = 14.0f;

    const float maxAngleX = DirectX::XMConvertToRadians(45);
    const float minAngleX = DirectX::XMConvertToRadians(-45);

    DirectX::XMFLOAT3 afterPerspective = { 0, 0, 0 };
    DirectX::XMFLOAT3 perspectiveq;

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
    // ���`��ԑ��x
    float lerpSpeed = 4.0f;

    // ���`��ԃt���O
    bool lerpFlag;

    DirectX::XMFLOAT3 shakePower = {1.0f, 1.0f, 1.0f};
    float shakesuppress = 0.01f;
    float shakeTimer = 0.0f;
};