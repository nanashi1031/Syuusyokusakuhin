#include "Graphics/Graphics.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyBoss.h"
#include "PlayerManager.h"
#include "SceneGame.h"
#include "StageMain.h"

void SceneGame::Initialize()
{
	// ステージ初期化
	// スマートポインタのほうがいい
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// カメラ
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f, 1000.0f
	);
	cameraController = new CameraController();

	// プレイヤー
	PlayerManager& playerManager = PlayerManager::Instance();
	Player* player = new Player;
	playerManager.Register(player);

	//エネミー
	for (int i = 0; i < 2; i++)
	{
		EnemyManager& enemyManager = EnemyManager::Instance();
		EnemyBoss* boss = new EnemyBoss();
		boss->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 10.0f));
		enemyManager.Register(boss);
	}

	EnemyManager& enemyManager = EnemyManager::Instance();
	EnemySlime* slime = new EnemySlime;
	slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
	enemyManager.Register(slime);
}

void SceneGame::Finalize()
{
	// 終了化

	StageManager::Instance().Clear();

	//カメラコントローラー
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//プレイヤー
	PlayerManager::Instance().Clear();

	//エネミー
	EnemyManager::Instance().Clear();
}

void SceneGame::Update(float elapsedTime)
{
	StageManager::Instance().Update(elapsedTime);

	cameraController->Update(elapsedTime);
	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
	// 腰当たりに設定
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	PlayerManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);
}

void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	// カメラ
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		StageManager::Instance().Render(dc, shader);

		PlayerManager::Instance().Render(dc, shader);

		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		PlayerManager::Instance().DrawDebugPrimitive();

		EnemyManager::Instance().DrawDebugPrimitive();
	}

	// 2Dスプライト描画
	{

	}

	// 2DデバッグGUI描画
	{
#if !DEBUG
		float alpha = 0.35f;
		ImGui::SetNextWindowBgAlpha(alpha);

		ImGui::Begin("ImGuiManager");
		{
			//bool imguiPlayer = false;
			//if (ImGui::Button("Player"))
			//{
			//	imguiPlayer = true;
			//}
			//if (imguiPlayer)
			//{

			//}
		}
		PlayerManager::Instance().DrawDebugGUI();
		cameraController->DrawDebugGUI();
		EnemyManager::Instance().DrawDebugGUI();

		ImGui::End();
#endif
	}
}
