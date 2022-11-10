#pragma once
#include"Graphics.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "StageManager.h"
#include "CameraController.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	// ���b�N�I���^�[�Q�b�g�����O�`��
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	CameraController* cameraController = nullptr;

	std::unique_ptr<Sprite> targetRing = nullptr;
};
