#pragma once
#include"Graphics.h"
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

private:
	CameraController* cameraController = nullptr;
};
