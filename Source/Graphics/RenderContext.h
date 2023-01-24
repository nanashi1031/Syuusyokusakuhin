#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// 平行光源情報
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
};

// 点光源情報
struct PointLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	float range;
	DirectX::XMFLOAT3 dummy;
};

static constexpr int PointLightMax = 8;

// スポットライト情報
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

// 色調補正情報
struct ColorGradingData
{
	float hueShift = 0;	// 色相調整
	float saturation = 1;	// 彩度調整
	float brightness = 1;	// 明度調整
	float dummy;
};

// ガウスフィルター計算情報
struct GaussianFilterData
{
	int kernelSize = 8; // カーネルサイズ
	float deviation = 10.0f; // 標準偏差
	DirectX::XMFLOAT2 textureSize; // 暈すテクスチャのサイズ
};
// ガウスフィルターの最大カーネルサイズ
static const int MaxKernelSize = 16;

// 高輝度抽出用情報
struct LuminanceExtractionData
{
	float threshold = 0.0f;	// 閾値
	float intensity = 0.00f;// ブルームの強度
	DirectX::XMFLOAT2 dummy2;
};

//	シャドウマップ用情報
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; //	シャドウマップテクスチャ
	DirectX::XMFLOAT4X4 lightViewProjection; //	ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor; // 影の色
	float shadowBias; // 深度比較用のオフセット値
};

// ポストエフェクトの最終パス用情報
struct FinalpassnData
{
	// ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};

// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	// カメラ情報
	DirectX::XMFLOAT4 viewPosition;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 lightDirection;

	// ライト情報
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData pointLightData[PointLightMax];
	SpotLightData spotLightData[SpotLightMax];
	int pointLightCount = 0;
	int spotLightCount = 0;

	// 色調補正情報
	ColorGradingData colorGradingData;

	// ガウスフィルター情報
	GaussianFilterData gaussianFilterData;

	// 高輝度抽出用情報
	LuminanceExtractionData	luminanceExtractionData;

	//	シャドウマップ情報
	ShadowMapData shadowMapData;

	// 最終パス情報
	FinalpassnData finalpassnData;
};
