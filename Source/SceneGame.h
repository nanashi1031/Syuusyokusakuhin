#pragma once
#include"Graphics\Graphics.h"
#include "Stage\Stage.h"

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
	Stage* stage = nullptr;
};
