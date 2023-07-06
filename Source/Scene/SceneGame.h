#pragma once
#include "Graphics.h"
#include "Texture.h"
#include "Sprite.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"
#include "RenderTarget.h"
#include "PostprocessingRenderer.h"
#include "StageMain.h"

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
	void UpdateHPBar();

private:
	// 3D��Ԃ̕`��
	void Render3DScene();

	void RenderShadowmap();

	// ���b�N�I���^�[�Q�b�g�����O�`��
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

	// HP�o�[
	void RenderHPBar(ID3D11DeviceContext* dc);
	void RenderButterfly(ID3D11DeviceContext* dc);
	void RenderPauseUI(ID3D11DeviceContext* dc);
	void RenderGameOverUI(ID3D11DeviceContext* dc);

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Sprite>	sprite;

	std::unique_ptr<Sprite> spr_gauge = nullptr;
	std::unique_ptr<Sprite> spr_hpFrame = nullptr;
	std::unique_ptr<Sprite> spr_hpBarRed = nullptr;
	std::unique_ptr<Sprite> spr_hpBarGreen = nullptr;

	std::unique_ptr<Sprite> spr_targetRing = nullptr;
	std::unique_ptr<Sprite> spr_butterfly = nullptr;

	std::unique_ptr<Sprite> spr_yazirushi = nullptr;
	std::unique_ptr<Sprite> spr_pause = nullptr;
	std::unique_ptr<Sprite> spr_modoru = nullptr;
	std::unique_ptr<Sprite> spr_titlehe = nullptr;
	std::unique_ptr<Sprite> spr_gameOver = nullptr;
	std::unique_ptr<Sprite> spr_retry = nullptr;
	std::unique_ptr<Sprite> spr_retire = nullptr;

	std::unique_ptr<StageMain> stageMain = nullptr;

	// ���s�����f�[�^
	std::unique_ptr<Light> directional_light = nullptr;
	DirectX::XMFLOAT4 ambientLightColor;

	// �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget = nullptr;

	//	�V���h�E�}�b�v�p���
	std::unique_ptr<Light> mainDirectionalLight = nullptr; // �V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil = nullptr; //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float shadowDrawRect = 120.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4	lightViewProjection; //	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f }; // �e�̐F
	float shadowBias = 0.001f; // �[�x��r�p�̃I�t�Z�b�g�l

	// �p�m���}�X�J�C�{�b�N�X�摜
	std::unique_ptr<Texture> skyboxTexture = nullptr;

	// �|�X�g�v���Z�X
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer = nullptr;

	// HP�o�[�̉���
	float playerHpRatio = 0.0f;
	float enemyBossHpRatio = 0.0f;
	float color = 0.0f;
	float colorFlag = false;

	bool sentaku = true;
	float targetRingAngle = 0.0f;

	// imgui�����p�ϐ�
	float a, b, c, d, e, f, g, h = 2.0f;
};
