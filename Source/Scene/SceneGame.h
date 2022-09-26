#pragma once
#include"Graphics.h"
#include "Graphics/Sprite.h"
#include "StageManager.h"
#include "CameraController.h"

// ゲームシーン
class SceneGame
{
public:
	SceneGame() {}
	~SceneGame() {}

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// ロックオンターゲットリング描画
	void RenderLockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);

private:
	CameraController* cameraController = nullptr;

	std::unique_ptr<Sprite> targetRing = nullptr;
};
