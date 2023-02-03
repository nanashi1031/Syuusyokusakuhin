#pragma once
#include"Graphics.h"
#include "Sprite.h"
#include "Texture.h"
#include "Sprite.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"
#include "RenderTarget.h"
#include "PostprocessingRenderer.h"

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

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Sprite>	sprite;

	std::unique_ptr<Sprite> gauge = nullptr;
	std::unique_ptr<Sprite> hpFrame = nullptr;
	std::unique_ptr<Sprite> hpBar_red = nullptr;

	std::unique_ptr<Sprite> targetRing = nullptr;
	std::unique_ptr<Sprite> butterfly = nullptr;

	// ���s�����f�[�^
	std::unique_ptr<Light> directional_light;
	DirectX::XMFLOAT4 ambientLightColor;

	// �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget;

	//	�V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr; // �V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float shadowDrawRect = 120.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4	lightViewProjection; //	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f }; // �e�̐F
	float shadowBias = 0.001f; // �[�x��r�p�̃I�t�Z�b�g�l

	// �p�m���}�X�J�C�{�b�N�X�摜
	std::unique_ptr<Texture> skyboxTexture;

	// �|�X�g�v���Z�X
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

	// HP�o�[�̉���
	float playerHpRatio = 0.0f;
	float enemyBossHpRatio = 0.0f;
	float color = 0.0f;
	float colorFlag = false;
};
