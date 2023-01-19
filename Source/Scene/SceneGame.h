#pragma once
#include"Graphics.h"
#include "Sprite.h"
#include "Texture.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"

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

private:
	// 3D��Ԃ̕`��
	void Render3DScene();

	void RenderShadowmap();

	// ���b�N�I���^�[�Q�b�g�����O�`��
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Sprite>	sprite;

	std::unique_ptr<Sprite> targetRing = nullptr;

	// ���s�����f�[�^
	std::unique_ptr<Light>		directional_light;
	DirectX::XMFLOAT4			ambientLightColor;

	// �K�E�X�t�B���^�[�f�[�^
	GaussianFilterData			gaussianFilterData;
	std::unique_ptr<Sprite>		gaussianBlurSprite;

	// �u���[���f�[�^
	LuminanceExtractionData		luminanceExtractionData;

	//	�V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr;
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float	shadowDrawRect = 500.0f; //	�V���h�E�}�b�v�ɕ`�悷��͈�

	//	�p�m���}�X�J�C�{�b�N�X�摜
	std::unique_ptr<Texture>	skyboxTexture;
};
