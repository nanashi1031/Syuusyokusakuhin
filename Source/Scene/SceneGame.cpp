#include "SceneGame.h"
#include "Input.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "EnemyBoss.h"
#include "EnemyPurpleDragon.h"
#include "PlayerManager.h"
#include "InsectManager.h"
#include "StageMain.h"
#include "StageManager.h"
#include "LightManager.h"
#include "Mathf.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneClear.h"
#include "EffectManager.h"

//	シャドウマップのサイズ
static const UINT SHADOWMAP_SIZE = 2048;

void SceneGame::Initialize()
{
	CameraController::Instance().SetCameraMouseOperationFlag(true);

	// ステージ初期化
	// スマートポインタのほうがいい
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// プレイヤー
	{
		PlayerManager& playerManager = PlayerManager::Instance();
		Player* player = new Player;
		playerManager.Register(player);
	}

	EnemyManager& enemyManager = EnemyManager::Instance();

	//エネミー
	{
		EnemyPurpleDragon* purpleDragon = new EnemyPurpleDragon();
		purpleDragon->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
		enemyManager.Register(purpleDragon);
	}

	// 虫
	{
		Insect* insect = new Insect;
		insect->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
		insect->SetScale(DirectX::XMFLOAT3(100, 100, 100));
		InsectManager::Instance().Register(insect);
	}

	// カメラ
	{
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
	}

	// スプライト
	hpFrame = std::make_unique<Sprite>("Data/Sprite/UI/HPFrame.png");
	hpBar_red = std::make_unique<Sprite>("Data/Sprite/UI/HPBar.png");

	targetRing = std::make_unique<Sprite>("Data/Sprite/UI/Ring1.png");
	butterfly = std::make_unique<Sprite>("Data/Sprite/UI/Butterfly.png");
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetPosition({ 0.70f, 0.60f, 0.30f });
		mainDirectionalLight->SetDirection({ -0.286f, -0.775f, 0.564f });
		mainDirectionalLight->SetColor({0, 0, 0, 1});
		LightManager::Instane().Register(mainDirectionalLight);
	}
	// 点光源
	{
		//Light* light = new Light(LightType::Point);
		//light->SetPosition(DirectX::XMFLOAT3(4, 1, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//LightManager::Instane().Register(light);
	}

	// スポットライト
	{
		//Light* light = new Light(LightType::Spot);
		//light->SetPosition(DirectX::XMFLOAT3(-10, 6, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		//light->SetRange(10.0f);
		//LightManager::Instane().Register(light);
	}

	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget =
			std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	// シャドウマップ用に深度ステンシルの生成
	{
		shadowDrawRect = 120.0f; // シャドウマップに描画する範囲
		shadowColor = { 0.7f, 0.7f, 0.7f }; // 影の色
		shadowBias = 0.001f; // 深度比較用のオフセット値

		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil =
			std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}
	Graphics& graphics = Graphics::Instance();
	// スカイボックス
	{
		Graphics& graphics = Graphics::Instance();
		skyboxTexture = std::make_unique<Texture>(
			"Data/Sprite/SkyTexture.png");
		sprite = std::make_unique<Sprite>();
		sprite->SetShaderResourceView(
			skyboxTexture->GetShaderResourceView(),
			skyboxTexture->GetWidth(), skyboxTexture->GetHeight());
		sprite->Update(
			0, 0,
			graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0,
			static_cast<float>(skyboxTexture->GetWidth()),
			static_cast<float>(skyboxTexture->GetHeight()),
			0,
			1, 1, 1, 1);
	}

	// ポストプロセス描画クラス生成
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// シーンテクスチャを設定しておく
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
}

void SceneGame::Finalize()
{
	// 終了化

	StageManager::Instance().Clear();

	PlayerManager::Instance().Clear();

	InsectManager::Instance().Clear();

	EnemyManager::Instance().Clear();

	LightManager::Instane().Clear();
}

void SceneGame::Update(float elapsedTime)
{
	StageManager::Instance().Update(elapsedTime);

	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
	// 腰当たりに設定
	target.y += PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetHeight();

	CameraController::Instance().SetTarget(target);
	CameraController::Instance().Update(elapsedTime);

	PlayerManager::Instance().Update(elapsedTime);

	InsectManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);

	UpdateHPBar();

	Player* player = PlayerManager::Instance().GetPlayer(0);
	if (player->GetHealthPercentage() < 0.5f)
	{
		if (colorFlag)
		{
			postprocessingRenderer->SetColorGradingData(color += 0.02f);
			if (color > 0.6f)
				colorFlag = false;
		}
		else if(!colorFlag)
		{
			postprocessingRenderer->SetColorGradingData(color -= 0.02f);
			if (color <= 0.0f)
				colorFlag = true;
		}
		//postprocessingRenderer->SetColorGradingData(player->GetHealthPercentage() - 0.5f);
	}
	else
		postprocessingRenderer->SetColorGradingData(1.0f);

	// Debug 本番では常にマウスを真ん中に固定する
	// TODO ポーズ中はマウス操作できるようにする
	Mouse& mouse = Input::Instance().GetMouse();
	if (CameraController::Instance().GetCameraMouseOperationFlag())
		mouse.SetMiddlePosition();

	if (EnemyManager::Instance().GetEnemyCount() == 0)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneClear));
	}
}

void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	RenderShadowmap();
	// 3D空間の描画を別のバッファに対して行う
	Render3DScene();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// ライト情報を詰め込む
	LightManager::Instane().PushRenderContext(rc);

	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	{
		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// ビューポートの設定
		D3D11_VIEWPORT	vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);

		//	ポストプロセス処理を行う
		postprocessingRenderer->Render(dc);
	}

	// 2Dスプライト描画
	{
		Camera& camera = Camera::Instance();
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();
		RenderHPBar(dc);

		RenderButterfly(dc);

		if (CameraController::Instance().GetLockOnFlag())
			RenderLockOn(dc, rc.view, rc.projection);

		Extract::Instance().Render2D(dc);
	}

	// 2DデバッグGUI描画
	{
#ifdef _DEBUG
		Extract::Instance().DrawDebugGUI();
		StageManager::Instance().DrawDebugGUI();
		PlayerManager::Instance().DrawDebugGUI();
		InsectManager::Instance().DrawDebugGUI();
		CameraController::Instance().DrawDebugGUI();
		EnemyManager::Instance().DrawDebugGUI();
		LightManager::Instane().DrawDebugGUI();
		ImGui::Separator();
		float alpha = 0.35f;
		ImGui::SetNextWindowBgAlpha(alpha);
		if (ImGui::TreeNode("Shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::ColorEdit3("Color", &shadowColor.x);
			ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->
				GetShaderResourceView().Get(),
				{ 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

			ImGui::TreePop();
		}
		ImGui::Separator();
		ImGui::SetNextWindowBgAlpha(alpha);
		postprocessingRenderer->DrawDebugGUI();
		ImGui::Separator();
#endif
	}
}

void SceneGame::UpdateHPBar()
{
	PlayerManager& playerManger = PlayerManager::Instance();
	Player* player = playerManger.GetPlayer(playerManger.GetplayerOneIndex());
	playerHpRatio = player->GetHealthPercentage();

	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		if (enemy->GetCharacterType() == 1)
			enemyBossHpRatio = Mathf::Percentage(enemy->GetHealth(), enemy->GetMaxHealth());
	}
}

// 3D空間の描画
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// ライトの情報を詰め込む
	LightManager::Instane().PushRenderContext(rc);

	// シャドウマップの設定
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	// カメラ
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// スカイボックスの描画
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
	}

	// 3Dモデル描画
	{
		ModelShader* modelShader = graphics.GetShader(ModelShaderId::Phong);
		modelShader->Begin(rc);

		StageManager::Instance().Render(rc, modelShader);

		PlayerManager::Instance().Render(rc, modelShader);

		InsectManager::Instance().Render(rc, modelShader);

		EnemyManager::Instance().Render(rc, modelShader);

		modelShader->End(rc);
	}

	EffectManager::Instance().Render(rc.view, rc.projection);

	// 3Dデバッグ描画
	{
#ifdef _DEBUG
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
		PlayerManager::Instance().DrawDebugPrimitive();
		EnemyManager::Instance().DrawDebugPrimitive();
		LightManager::Instane().DrawDebugPrimitive();
#endif
	}
}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	// レンダーターゲット設定
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// カメラパラメータ設定
	{
		// 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}

	// 3Dモデル描画
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
		shader->Begin(rc);

		StageManager& stageManager = StageManager::Instance();
		PlayerManager& playerManager = PlayerManager::Instance();
		EnemyManager& enemyManager = EnemyManager::Instance();
		for (int i = 0; i < stageManager.GetStageCount(); i++)
		{
			shader->Draw(rc, stageManager.GetStage(i)->GetModel());
		}
		for (int i = 0; i < playerManager.GetPlayerCount(); i++)
		{
			shader->Draw(rc, playerManager.GetPlayer(i)->GetModel());

		}
		for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
		{
			shader->Draw(rc, enemyManager.GetEnemy(i)->GetModel());
		}
		InsectManager& insectManager = InsectManager::Instance();
		for (int i = 0; i < insectManager.GetInsectCount(); i++)
		{
			int state =
				InsectManager::Instance().GetInsect(i)->GetStateMachine()->GetStateIndex();
			if (state != static_cast<int>(Insect::State::Pursuit))
				shader->Draw(rc, insectManager.GetInsect(i)->GetModel());
		}
		shader->End(rc);
	}
}

void SceneGame::RenderLockOn(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	Enemy* enemy = EnemyManager::Instance().GetEnemy(0);

	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numVieports = 1;
	dc->RSGetViewports(&numVieports, &viewport);
	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// エネミーのターゲット位置のワールド座標
	CameraController& cameraController = CameraController::Instance();
	int targetIndex =
		cameraController.GetTargets()[cameraController.GetTagetIndex()].index;;
	Model::Node* node =
		enemy->GetModel()->FindNode(enemy->GetParts()[targetIndex].name);
	DirectX::XMFLOAT3 worldPosition =
		enemy->GetNodePosition(node);
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
	// ワールド座標からスクリーン座標へ変換
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,			//ワールド座標
		viewport.TopLeftX,		//ビューポート左上X位置
		viewport.TopLeftY,		//ビューポート左上Y位置
		viewport.Width,			//ビューポート幅
		viewport.Height,		//ビューポート高さ
		viewport.MinDepth,		//深度幅の範囲を表す最小値
		viewport.MaxDepth,		//深度幅の範囲を表す最大値
		Projection,				//プロジェクション行列
		View,					//ビュー行列
		World					//ワールド行列
	);

	// スクリーン座標
	DirectX::XMFLOAT2 screenPosition;
	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	float textureWidth = static_cast<float>(targetRing->GetTextureWidth());
	float textureHeight = static_cast<float>(targetRing->GetTextureHeight());
	targetRing->Render(
		dc,
		screenPosition.x - (textureWidth / 3 / 2), screenPosition.y - (textureHeight / 3 / 2),
		textureWidth / 3, textureHeight / 3,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);
	float o = screenPosition.x - (textureWidth / 3 / 2);
	float oo = screenPosition.y - (textureHeight / 3 / 2);
	targetRing->Render(
		dc,
		screenPosition.x - (textureWidth / 3 / 2), screenPosition.y - (textureHeight / 3 / 2),
		textureWidth / 3, textureHeight / 3,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);
}

void SceneGame::RenderHPBar(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(hpFrame->GetTextureWidth());
	float textureHeight = static_cast<float>(hpFrame->GetTextureHeight());
	// プレイヤーHPのフレーム
	hpFrame->Render(
		dc,
		screenWidth / -55, screenHeight / -20,
		textureWidth / 1.5f, textureHeight / 2.5f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	// エネミーHPのフレーム
	hpFrame->Render(
		dc,
		screenWidth / 10, screenHeight / 1.3f,
		(textureWidth * 3) / 2, textureHeight / 1.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(hpBar_red->GetTextureWidth());
	textureHeight = static_cast<float>(hpBar_red->GetTextureHeight());
	// プレイヤーのHP
	hpBar_red->Render(
		dc,
		screenWidth / -55, screenHeight / -20,
		playerHpRatio * (textureWidth / 1.5f), textureHeight / 2.5f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	// ボスのHP
	hpBar_red->Render(
		dc,
		screenWidth / 10, screenHeight / 1.3f,
	    //150, 650,
		enemyBossHpRatio * (textureWidth * 3) / 2, textureHeight / 1.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);
}

void SceneGame::RenderButterfly(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenWidth());
	float textureWidth = static_cast<float>(butterfly->GetTextureWidth());
	float textureHeight = static_cast<float>(butterfly->GetTextureHeight());

	DirectX::XMFLOAT4 color = Extract::Instance().ColorConversion(
		InsectManager::Instance().GetInsect(0)->GetExtractColor());

	butterfly->Render(
		dc,
		screenWidth / 40, screenHeight / 18,
		textureWidth / 4, textureHeight / 4,
		0, 0,
		textureWidth, textureHeight,
		0,
		color.x, color.y, color.z, color.w
	);
}