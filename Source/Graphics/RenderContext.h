#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// ���s�������
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
};

// �_�������
struct PointLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	float range;
	DirectX::XMFLOAT3 dummy;
};

static constexpr int PointLightMax = 8;

// �X�|�b�g���C�g���
struct SpotLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
	float range;
	float innerCorn;
	float outerCorn;
	float dummy;
};
static constexpr int SpotLightMax = 8;

// �F���␳���
struct ColorGradingData
{
	float hueShift = 0;	// �F������
	float saturation = 1;	// �ʓx����
	float brightness = 1;	// ���x����
	float dummy;
};

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int kernelSize = 8; // �J�[�l���T�C�Y
	float deviation = 10.0f; // �W���΍�
	DirectX::XMFLOAT2 textureSize; // �򂷃e�N�X�`���̃T�C�Y
};
// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float threshold = 0.0f;	// 臒l
	float intensity = 0.00f;// �u���[���̋��x
	DirectX::XMFLOAT2 dummy2;
};

//	�V���h�E�}�b�v�p���
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; //	�V���h�E�}�b�v�e�N�X�`��
	DirectX::XMFLOAT4X4 lightViewProjection; //	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor; // �e�̐F
	float shadowBias; // �[�x��r�p�̃I�t�Z�b�g�l
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassnData
{
	// �u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	// �J�������
	DirectX::XMFLOAT4 viewPosition;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 lightDirection;

	// ���C�g���
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData pointLightData[PointLightMax];
	SpotLightData spotLightData[SpotLightMax];
	int pointLightCount = 0;
	int spotLightCount = 0;

	// �F���␳���
	ColorGradingData colorGradingData;

	// �K�E�X�t�B���^�[���
	GaussianFilterData gaussianFilterData;

	// ���P�x���o�p���
	LuminanceExtractionData	luminanceExtractionData;

	//	�V���h�E�}�b�v���
	ShadowMapData shadowMapData;

	// �ŏI�p�X���
	FinalpassnData finalpassnData;
};
