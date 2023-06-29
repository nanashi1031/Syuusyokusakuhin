#pragma once

#include "Sprite.h"
#include "Texture.h"
#include "Scene.h"
#include "DepthStencil.h"
#include "Light.h"
#include "RenderTarget.h"
#include "PostprocessingRenderer.h"
#include "Audio.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

private:
    void Render3DScene();

private:
    std::unique_ptr<Sprite>	sprite;
    std::unique_ptr<Sprite> spr_title = nullptr;

    std::unique_ptr<AudioSource> SE_Kettei;

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
};