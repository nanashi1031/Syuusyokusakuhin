#pragma once
#include"Graphics.h"
#include "Graphics/Sprite.h"
#include "StageManager.h"
#include "CameraController.h"

// �Q�[���V�[��
class SceneGame
{
public:
	SceneGame() {}
	~SceneGame() {}

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render();

	// ���b�N�I���^�[�Q�b�g�����O�`��
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	CameraController* cameraController = nullptr;

	std::unique_ptr<Sprite> targetRing = nullptr;
};
