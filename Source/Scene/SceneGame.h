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

	// 平行光源データ
	std::unique_ptr<Light> directional_light = nullptr;
	DirectX::XMFLOAT4 ambientLightColor;

	// オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget = nullptr;

	//	シャドウマップ用情報
	std::unique_ptr<Light> mainDirectionalLight = nullptr; // シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil = nullptr; //	シャドウマップ用深度ステンシルバッファ
	float shadowDrawRect = 120.0f; // シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4	lightViewProjection; //	ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f }; // 影の色
	float shadowBias = 0.001f; // 深度比較用のオフセット値

	// パノラマスカイボックス画像
	std::unique_ptr<Texture> skyboxTexture = nullptr;

	// ポストプロセス
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer = nullptr;

	// HPバーの横幅
	float playerHpRatio = 0.0f;
	float enemyBossHpRatio = 0.0f;
	float color = 0.0f;
	float colorFlag = false;

	bool sentaku = true;
	float targetRingAngle = 0.0f;

	// imgui調整用変数
	float a, b, c, d, e, f, g, h = 2.0f;
};
