#pragma once

#include "Sprite.h"
#include "Texture.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"
#include "RenderTarget.h"
#include "PostprocessingRenderer.h"
#include "Audio.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    void Render3DScene();

private:
    std::unique_ptr<Sprite>	sprite;
    std::unique_ptr<Sprite> spr_title = nullptr;

    std::unique_ptr<AudioSource> SE_Kettei;

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
};