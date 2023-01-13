#include "Graphics/Graphics.h"
#include "Camera.h"
#include "EnemyManager.h""
#include "EnemyBoss.h"
#include "EnemyPurpleDragon.h"
#include "PlayerManager.h"
#include "InsectManager.h"
#include "LightManager.h"
#include "SceneGame.h"
#include "StageMain.h"

//	シャドウマップのサイズ
static	const	UINT	SHADOWMAP_SIZE = 2048;

void SceneGame::Initialize()
{
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
		cameraController = new CameraController();
	}

	// スプライト
	targetRing = std::make_unique<Sprite>("Data/Sprite/Ring.png");

	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1, -1, -1 });
		LightManager::Instane().Register(mainDirectionalLight);
	}
	// 点光源
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(4, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instane().Register(light);
	}

	// スポットライト
	{
		Light* light = new Light(LightType::Spot);
		light->SetPosition(DirectX::XMFLOAT3(-10, 6, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		light->SetRange(10.0f);
		LightManager::Instane().Register(light);
	}

	// ガウスブラー
	{
		texture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");

		gaussianBlurSprite = std::make_unique<Sprite>();
		gaussianBlurSprite->SetShaderResourceView(
			texture->GetShaderResourceView(),
			texture->GetWidth(), texture->GetHeight());

	}

	// シャドウマップ用に深度ステンシルの生成
	{
		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// スカイボックス
	{
		Graphics& graphics = Graphics::Instance();
		skyboxTexture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");
		sprite = std::make_unique<Sprite>();
		sprite->SetShaderResourceView(skyboxTexture->GetShaderResourceView(), skyboxTexture->GetWidth(), skyboxTexture->GetHeight());
		sprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(skyboxTexture->GetWidth()), static_cast<float>(skyboxTexture->GetHeight()),
			0,
			1, 1, 1, 1);
	}
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

	// 虫
	InsectManager::Instance().Clear();

	//エネミー
	EnemyManager::Instance().Clear();
}

void SceneGame::Update(float elapsedTime)
{
	StageManager::Instance().Update(elapsedTime);

	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
	// 腰当たりに設定
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	PlayerManager::Instance().Update(elapsedTime);

	InsectManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);

	gaussianBlurSprite->Update(0.0f, 0.0f,
		Graphics::Instance().GetScreenWidth(), Graphics::Instance().GetScreenHeight(),
		0.0f, 0.0f,
		static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
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
	rc.deviceContext = dc;

	// ライト情報を詰め込む
	LightManager::Instane().PushRenderContext(rc);

	// カメラ
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//RenderShadowmap();
	Render3DScene();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		ModelShader* modelShader = graphics.GetShader(ModelShaderId::Phong);
		modelShader->Begin(rc);

		StageManager::Instance().Render(rc, modelShader);

		PlayerManager::Instance().Render(rc, modelShader);

		InsectManager::Instance().Render(rc, modelShader);

		EnemyManager::Instance().Render(rc, modelShader);

		modelShader->End(rc);
	}

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

	// 2Dスプライト描画
	{
		if (cameraController->GetLockOnFlag())
			RenderLockOn(dc, rc.view, rc.projection);

		SpriteShader* shader = graphics.GetShader(SpriteShaderId::GaussianBlur);

		RenderContext rc;
		rc.deviceContext = dc;
		rc.gaussianFilterData = gaussianFilterData;
		rc.gaussianFilterData.textureSize.x = texture->GetWidth();
		rc.gaussianFilterData.textureSize.y = texture->GetHeight();
		shader->Begin(rc);

		//shader->Draw(rc, gaussianBlurSprite.get());

		shader->End(rc);
	}

	// 2DデバッグGUI描画
	{
#ifdef _DEBUG
		PlayerManager::Instance().DrawDebugGUI();
		InsectManager::Instance().DrawDebugGUI();
		cameraController->DrawDebugGUI();
		EnemyManager::Instance().DrawDebugGUI();
		LightManager::Instane().DrawDebugGUI();
		ImGui::Separator();
		if (ImGui::TreeNode("GaussianFilter"))
		{
			ImGui::SliderInt("kernelSize", &gaussianFilterData.kernelSize, 1, MaxKernelSize - 1);
			ImGui::SliderFloat("deviation", &gaussianFilterData.deviation, 1.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNode("Shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

			ImGui::TreePop();
		}
		ImGui::Separator();
#endif
	}
}

void SceneGame::RenderLockOn(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	Enemy* enemy = EnemyManager::Instance().GetEnemy(cameraController->GetTagetIndex());

	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numVieports = 1;
	dc->RSGetViewports(&numVieports, &viewport);
	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// エネミーの頭上のワールド座標
	DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	worldPosition.y += enemy->GetHeight();
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
}

// 3D空間の描画
void SceneGame:: Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// スカイボックスの描画
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
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
	}

	// 3Dモデル描画
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
		shader->Begin(rc);

		PlayerManager& playerManager = PlayerManager::Instance();
		//shader->Draw(rc, stage.get());
		shader->Draw(rc, playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetModel());
		//shader->Draw(rc, uncle.get());

		shader->End(rc);
	}
}