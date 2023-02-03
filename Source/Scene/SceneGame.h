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
	void UpdateHPBar();

private:
	// 3D空間の描画
	void Render3DScene();

	void RenderShadowmap();

	// ロックオンターゲットリング描画
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

	// HPバー
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

	// 平行光源データ
	std::unique_ptr<Light> directional_light;
	DirectX::XMFLOAT4 ambientLightColor;

	// オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;

	//	シャドウマップ用情報
	Light* mainDirectionalLight = nullptr; // シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; //	シャドウマップ用深度ステンシルバッファ
	float shadowDrawRect = 120.0f; // シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4	lightViewProjection; //	ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f }; // 影の色
	float shadowBias = 0.001f; // 深度比較用のオフセット値

	// パノラマスカイボックス画像
	std::unique_ptr<Texture> skyboxTexture;

	// ポストプロセス
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;

	// HPバーの横幅
	float playerHpRatio = 0.0f;
	float enemyBossHpRatio = 0.0f;
	float color = 0.0f;
	float colorFlag = false;
};
