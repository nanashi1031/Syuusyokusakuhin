#pragma once
#include"Graphics.h"
#include "Sprite.h"
#include "Texture.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	// 3D空間の描画
	void Render3DScene();

	void RenderShadowmap();

	// ロックオンターゲットリング描画
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Sprite>	sprite;

	std::unique_ptr<Sprite> targetRing = nullptr;

	// 平行光源データ
	std::unique_ptr<Light>		directional_light;
	DirectX::XMFLOAT4			ambientLightColor;

	// ガウスフィルターデータ
	GaussianFilterData			gaussianFilterData;
	std::unique_ptr<Sprite>		gaussianBlurSprite;

	// ブルームデータ
	LuminanceExtractionData		luminanceExtractionData;

	//	シャドウマップ用情報
	Light* mainDirectionalLight = nullptr;
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; //	シャドウマップ用深度ステンシルバッファ
	float	shadowDrawRect = 500.0f; //	シャドウマップに描画する範囲

	//	パノラマスカイボックス画像
	std::unique_ptr<Texture>	skyboxTexture;
};
