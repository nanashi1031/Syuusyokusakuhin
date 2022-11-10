#pragma once
#include"Graphics.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "StageManager.h"
#include "CameraController.h"

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

	// ロックオンターゲットリング描画
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	CameraController* cameraController = nullptr;

	std::unique_ptr<Sprite> targetRing = nullptr;
};
