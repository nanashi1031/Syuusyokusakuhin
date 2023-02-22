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

//	�V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;

void SceneGame::Initialize()
{
	CameraController::Instance().SetCameraMouseOperationFlag(true);

	// �X�e�[�W������
	// �X�}�[�g�|�C���^�̂ق�������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// �v���C���[
	{
		PlayerManager& playerManager = PlayerManager::Instance();
		Player* player = new Player;
		playerManager.Register(player);
	}

	EnemyManager& enemyManager = EnemyManager::Instance();

	//�G�l�~�[
	{
		EnemyPurpleDragon* purpleDragon = new EnemyPurpleDragon();
		purpleDragon->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
		enemyManager.Register(purpleDragon);
	}

	// ��
	{
		Insect* insect = new Insect;
		insect->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
		insect->SetScale(DirectX::XMFLOAT3(100, 100, 100));
		InsectManager::Instance().Register(insect);
	}

	// �J����
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

	// �X�v���C�g
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
	// �_����
	{
		//Light* light = new Light(LightType::Point);
		//light->SetPosition(DirectX::XMFLOAT3(4, 1, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//LightManager::Instane().Register(light);
	}

	// �X�|�b�g���C�g
	{
		//Light* light = new Light(LightType::Spot);
		//light->SetPosition(DirectX::XMFLOAT3(-10, 6, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		//light->SetRange(10.0f);
		//LightManager::Instane().Register(light);
	}

	// �V�����`��^�[�Q�b�g�̐���
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget =
			std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	// �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
	{
		shadowDrawRect = 120.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
		shadowColor = { 0.7f, 0.7f, 0.7f }; // �e�̐F
		shadowBias = 0.001f; // �[�x��r�p�̃I�t�Z�b�g�l

		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil =
			std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}
	Graphics& graphics = Graphics::Instance();
	// �X�J�C�{�b�N�X
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

	// �|�X�g�v���Z�X�`��N���X����
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// �V�[���e�N�X�`����ݒ肵�Ă���
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
}

void SceneGame::Finalize()
{
	// �I����

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
	// ��������ɐݒ�
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

	// Debug �{�Ԃł͏�Ƀ}�E�X��^�񒆂ɌŒ肷��
	// TODO �|�[�Y���̓}�E�X����ł���悤�ɂ���
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
	// 3D��Ԃ̕`���ʂ̃o�b�t�@�ɑ΂��čs��
	Render3DScene();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// ���C�g�����l�ߍ���
	LightManager::Instane().PushRenderContext(rc);

	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT	vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);

		//	�|�X�g�v���Z�X�������s��
		postprocessingRenderer->Render(dc);
	}

	// 2D�X�v���C�g�`��
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

	// 2D�f�o�b�OGUI�`��
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

// 3D��Ԃ̕`��
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// ���C�g�̏����l�ߍ���
	LightManager::Instane().PushRenderContext(rc);

	// �V���h�E�}�b�v�̐ݒ�
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	// �J����
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// �X�J�C�{�b�N�X�̕`��
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
	}

	// 3D���f���`��
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

	// 3D�f�o�b�O�`��
	{
#ifdef _DEBUG
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// �f�o�b�O�����_���`����s
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

	// ��ʃN���A
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	// �����_�[�^�[�Q�b�g�ݒ�
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// �J�����p�����[�^�ݒ�
	{
		// ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}

	// 3D���f���`��
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

	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numVieports = 1;
	dc->RSGetViewports(&numVieports, &viewport);
	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// �G�l�~�[�̃^�[�Q�b�g�ʒu�̃��[���h���W
	CameraController& cameraController = CameraController::Instance();
	int targetIndex =
		cameraController.GetTargets()[cameraController.GetTagetIndex()].index;;
	Model::Node* node =
		enemy->GetModel()->FindNode(enemy->GetParts()[targetIndex].name);
	DirectX::XMFLOAT3 worldPosition =
		enemy->GetNodePosition(node);
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
	// ���[���h���W����X�N���[�����W�֕ϊ�
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,			//���[���h���W
		viewport.TopLeftX,		//�r���[�|�[�g����X�ʒu
		viewport.TopLeftY,		//�r���[�|�[�g����Y�ʒu
		viewport.Width,			//�r���[�|�[�g��
		viewport.Height,		//�r���[�|�[�g����
		viewport.MinDepth,		//�[�x���͈̔͂�\���ŏ��l
		viewport.MaxDepth,		//�[�x���͈̔͂�\���ő�l
		Projection,				//�v���W�F�N�V�����s��
		View,					//�r���[�s��
		World					//���[���h�s��
	);

	// �X�N���[�����W
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
	// �v���C���[HP�̃t���[��
	hpFrame->Render(
		dc,
		screenWidth / -55, screenHeight / -20,
		textureWidth / 1.5f, textureHeight / 2.5f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	// �G�l�~�[HP�̃t���[��
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
	// �v���C���[��HP
	hpBar_red->Render(
		dc,
		screenWidth / -55, screenHeight / -20,
		playerHpRatio * (textureWidth / 1.5f), textureHeight / 2.5f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	// �{�X��HP
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