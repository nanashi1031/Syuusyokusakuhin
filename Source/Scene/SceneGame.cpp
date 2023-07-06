#include "SceneGame.h"
#include "Input.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
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
#include "SceneTitle.h"
#include "EffectManager.h"

//	�V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;
static 	std::unique_ptr<Player> player = nullptr;
static std::unique_ptr<EnemyPurpleDragon> purpleDragon = nullptr;
static std::unique_ptr<Insect> insect = nullptr;

void SceneGame::Initialize()
{
	CameraController::Instance().SetCameraMouseOperationFlag(true);

	// �X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	stageMain = std::make_unique<StageMain>();
	stageManager.Register(stageMain.get());

	// TODO:���������[�N�������I�C���K�{�I
	// �搶�ɕ����Ă��C���s�\���������߂���������u�A���j�[�N�|�C���^�ɂ���ƃG���[
	{
		// �v���C���[
		{
			PlayerManager& playerManager = PlayerManager::Instance();
			player = std::make_unique<Player>();
			playerManager.Register(player.get());
		}

		EnemyManager& enemyManager = EnemyManager::Instance();

		//�G�l�~�[
		{
			purpleDragon = std::make_unique<EnemyPurpleDragon>();
			purpleDragon->SetPosition(DirectX::XMFLOAT3(2.0f, 0.0f, 10.0f));
			enemyManager.Register(purpleDragon.get());
		}

		// ��
		{
			insect = std::make_unique<Insect>();
			insect->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
			insect->SetScale(DirectX::XMFLOAT3(100, 100, 100));
			InsectManager::Instance().Register(insect.get());
		}
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
	{
	spr_hpFrame = std::make_unique<Sprite>("Data/Sprite/UI/HPFrame.png");
	spr_hpBarRed = std::make_unique<Sprite>("Data/Sprite/UI/HPBar.png");
	spr_hpBarGreen = std::make_unique<Sprite>("Data/Sprite/UI/HPBarPlayer.png");

	spr_targetRing = std::make_unique<Sprite>("Data/Sprite/UI/Ring.png");
	spr_butterfly = std::make_unique<Sprite>("Data/Sprite/UI/Butterfly.png");

	spr_yazirushi = std::make_unique<Sprite>("Data/Sprite/UI/Yazirushi.png");
	spr_pause = std::make_unique<Sprite>("Data/Sprite/Scene/Pause.png");
	spr_titlehe = std::make_unique<Sprite>("Data/Sprite/Scene/Titlehe.png");
	spr_modoru = std::make_unique<Sprite>("Data/Sprite/Scene/Modoru.png");
	spr_gameOver = std::make_unique<Sprite>("Data/Sprite/Scene/GameOver.png");
	spr_retry = std::make_unique<Sprite>("Data/Sprite/Scene/Retry.png");
	spr_retire = std::make_unique<Sprite>("Data/Sprite/Scene/Retire.png");
	}

	{
		mainDirectionalLight = std::make_unique<Light>(LightType::Directional);
		mainDirectionalLight->SetPosition({ 0.70f, 0.60f, 0.30f });
		mainDirectionalLight->SetDirection({ -0.286f, -0.775f, 0.564f });
		mainDirectionalLight->SetColor({0, 0, 0, 1});
		LightManager::Instane().Register(mainDirectionalLight.get());
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
		// �|�X�g�v���Z�X
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// �V�[���e�N�X�`����ݒ肵�Ă���
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}

	SceneManager::Instance().SetSceneGameState(SceneGameState::Game);
	sentaku = true;
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
	Player* player =
		PlayerManager::Instance().GetPlayer(
			PlayerManager::Instance().GetplayerOneIndex());
	switch (SceneManager::Instance().GetSceneGameState())
	{
	case SceneGameState::Game:
	{
		StageManager::Instance().Update(elapsedTime);

		DirectX::XMFLOAT3 target = player->GetPosition();

		// ��������ɐݒ�
		target.y += player->GetHeight();

		CameraController::Instance().SetTarget(target);
		CameraController::Instance().Update(elapsedTime);

		PlayerManager::Instance().Update(elapsedTime);

		InsectManager::Instance().Update(elapsedTime);

		EnemyManager::Instance().Update(elapsedTime);

		UpdateHPBar();

		// ��ʂ̓_�ŏ����֐��ɂ���
		// �v���C���[�̗̑͂������ȉ��Ȃ�
		if (player->GetHealthPercentage() < 0.5f)
		{
			if (colorFlag)
			{
				postprocessingRenderer->SetColorGradingData(color += 0.02f);
				if (color > 0.6f)
					colorFlag = false;
			}
			else if (!colorFlag)
			{
				postprocessingRenderer->SetColorGradingData(color -= 0.02f);
				if (color <= 0.0f)
					colorFlag = true;
			}
		}
		else
			postprocessingRenderer->SetColorGradingData(1.0f);

		// Debug �{�Ԃł͏�Ƀ}�E�X��^�񒆂ɌŒ肷��
		Mouse& mouse = Input::Instance().GetMouse();
		if (CameraController::Instance().GetCameraMouseOperationFlag())
			mouse.SetMiddlePosition();

		if (EnemyManager::Instance().GetEnemyCount() == 0)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneClear));
		}

		GamePad& gamePad = Input::Instance().GetGamePad();

		if ((gamePad.GetButtonDown() & gamePad.BTN_PAD_MENU) ||
			(gamePad.GetButtonDown() & gamePad.KEY_P))
		{
			SceneManager::Instance().SetSceneGameState(SceneGameState::Pause);
		}
		break;
	}
	case SceneGameState::Pause:
	{
		Mouse& mouse = Input::Instance().GetMouse();
		GamePad& gamePad = Input::Instance().GetGamePad();

		float ay = gamePad.GetAxisLY();
		// �X�e�B�b�N����ɓ|������W�L�[����������
		if (ay > 0.3f || (mouse.GetButtonDown() & gamePad.BTN_UP))
			sentaku = true;
		// �X�e�B�b�N�����ɓ|������S�L�[����������
		if (ay < -0.3f || (mouse.GetButtonDown() & gamePad.BTN_DOWN))
			sentaku = false;

		if ((gamePad.GetButtonDown() & gamePad.BTN_A) ||
			(gamePad.GetButtonDown() & gamePad.KEY_Z) ||
			(mouse.GetButtonDown() & mouse.BTN_LEFT))
		{
			if (sentaku)
			{
				SceneManager::Instance().SetSceneGameState(SceneGameState::Game);
			}
			else
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
	case SceneGameState::GameOver:
	{
		// �X�|�b�g���C�g
		{
			// �ォ��g���\��̂��ߎc��
#if 0
			Light* light = new Light(LightType::Spot);
			light->SetPosition(DirectX::XMFLOAT3(-10, 6, 0));
			light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
			light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
			light->SetRange(10.0f);
			LightManager::Instane().Register(light);
#endif
		}

		Mouse& mouse = Input::Instance().GetMouse();
		GamePad& gamePad = Input::Instance().GetGamePad();

		float ay = gamePad.GetAxisLY();
		// �X�e�B�b�N����ɓ|������W�L�[����������
		if (ay > 0.3f || (mouse.GetButtonDown() & gamePad.BTN_UP))
			sentaku = true;
		// �X�e�B�b�N�����ɓ|������S�L�[����������
		if (ay < -0.3f || (mouse.GetButtonDown() & gamePad.BTN_DOWN))
			sentaku = false;

		if ((gamePad.GetButtonDown() & gamePad.BTN_A) ||
			(gamePad.GetButtonDown() & gamePad.KEY_Z) ||
			(mouse.GetButtonDown() & mouse.BTN_LEFT))
		{
			if (sentaku)
			{
				// �̗�Max�A���G���ԕt�^���ĕ���
				player->SetHealth(player->GetMaxHealth());
				player->SetInvincibleTimer(5.0f);
				SceneManager::Instance().SetSceneGameState(SceneGameState::Game);
			}
			else
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
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
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
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
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(
			dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

		Player* player =
			PlayerManager::Instance().GetPlayer(
				PlayerManager::Instance().GetplayerOneIndex());

		if (SceneManager::Instance().GetSceneGameState() == SceneGameState::Pause)
		{
			RenderPauseUI(dc);
		}

		if (SceneManager::Instance().GetSceneGameState() == SceneGameState::GameOver)
		{
			RenderGameOverUI(dc);
		}

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
			ImGui::DragFloat("a", &a, 0.01f);
			ImGui::DragFloat("b", &b, 0.01f);
			ImGui::DragFloat("c", &c, 0.01f);
			ImGui::DragFloat("d", &d, 0.01f);
			ImGui::DragFloat("e", &e, 0.01f);
			ImGui::DragFloat("f", &f, 0.01f);
			ImGui::DragFloat("g", &g, 0.01f);
			ImGui::DragFloat("h", &h, 0.01f);

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
		if (enemy->GetCharacterType() == Character::Type::Boss)
		{
			enemyBossHpRatio =
				Mathf::Percentage(enemy->GetHealth(), enemy->GetMaxHealth());
		}
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

	// �G�l�~�[�̃^�[�Q�b�g�ʒu�̃��[���h���W
	CameraController& cameraController = CameraController::Instance();
	int targetIndex =
		cameraController.GetTargets()[cameraController.GetTagetIndex()].index;;
	Model::Node* node =
		enemy->GetModel()->FindNode(enemy->GetParts()[targetIndex].name);
	DirectX::XMFLOAT3 worldPosition =
		enemy->GetNodePosition(node);

	targetRingAngle += 1.0f;

	// �X�N���[�����W
	DirectX::XMFLOAT2 screenPosition = Mathf::ConvertWorldToScreen(worldPosition, dc, view, projection);

	float textureWidth = static_cast<float>(spr_targetRing->GetTextureWidth());
	float textureHeight = static_cast<float>(spr_targetRing->GetTextureHeight());
	spr_targetRing->Render(
		dc,
		screenPosition.x - (textureWidth / 3 / 2), screenPosition.y - (textureHeight / 3 / 2),
		textureWidth / 3, textureHeight / 3,
		0, 0, textureWidth , textureHeight,
		targetRingAngle,
		1, 1, 1, 1);
}

void SceneGame::RenderHPBar(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(spr_hpFrame->GetTextureWidth());
	float textureHeight = static_cast<float>(spr_hpFrame->GetTextureHeight());
	// �v���C���[HP�̃t���[��
	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_hpFrame->Render(
		dc,
		screenWidth / 40.0f, screenHeight / 17.0f,
		textureWidth, textureHeight,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	// �G�l�~�[HP�̃t���[��
	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_hpFrame->Render(
		dc,
		screenWidth / 6.0f, screenHeight / 1.2f,
		(textureWidth * 3) / 1.16f, textureHeight / 0.8f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_hpBarGreen->GetTextureWidth());
	textureHeight = static_cast<float>(spr_hpBarGreen->GetTextureHeight());
	// �v���C���[��HP
	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_hpBarGreen->Render(
		dc,
		screenWidth / 31.0f, screenHeight / 15.4f,
		playerHpRatio * textureWidth, textureHeight / 0.69f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_hpBarRed->GetTextureWidth());
	textureHeight = static_cast<float>(spr_hpBarRed->GetTextureHeight());
	// �{�X��HP
	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_hpBarRed->Render(
		dc,
		screenWidth / 5.52f, screenHeight / 1.193f,
		enemyBossHpRatio * textureWidth / 0.38f, textureHeight / 0.5f,
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
	float textureWidth = static_cast<float>(spr_butterfly->GetTextureWidth());
	float textureHeight = static_cast<float>(spr_butterfly->GetTextureHeight());

	DirectX::XMFLOAT4 color =
		Extract::Instance().ColorConversion(
		InsectManager::Instance().GetInsect(0)->GetExtractColor());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_butterfly->Render(
		dc,
		screenWidth / 40, screenHeight / 18,
		textureWidth / 4, textureHeight / 4,
		0, 0,
		textureWidth, textureHeight,
		0,
		color.x, color.y, color.z, color.w
	);
}

void SceneGame::RenderPauseUI(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenWidth());
	float textureWidth = static_cast<float>(spr_pause->GetTextureWidth());
	float textureHeight = static_cast<float>(spr_pause->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_pause->Render(
		dc,
		screenWidth / 3.0f, screenHeight / 12.0f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_yazirushi->GetTextureWidth());
	textureHeight = static_cast<float>(spr_yazirushi->GetTextureHeight());

	if (sentaku)
	{
		// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
		spr_yazirushi->Render(
			dc,
			screenWidth / 3.5f, screenHeight / 3.2f,
			textureWidth / 2.5f, textureHeight / 2.5f,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
	}
	else
	{
		// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
		spr_yazirushi->Render(
			dc,
			screenWidth / 3.5f, screenHeight / 2.6f,
			textureWidth / 2.5f, textureHeight / 2.5f,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
	}

	textureWidth = static_cast<float>(spr_modoru->GetTextureWidth());
	textureHeight = static_cast<float>(spr_modoru->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_modoru->Render(
		dc,
		screenWidth / 2.5f, screenHeight / 3.0f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_titlehe->GetTextureWidth());
	textureHeight = static_cast<float>(spr_titlehe->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_titlehe->Render(
		dc,
		screenWidth / 2.5f, screenHeight / 2.45f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);
}

void SceneGame::RenderGameOverUI(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenWidth());
	float textureWidth = static_cast<float>(spr_gameOver->GetTextureWidth());
	float textureHeight = static_cast<float>(spr_gameOver->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_gameOver->Render(
		dc,
		screenWidth / 3.0f, screenHeight / 12.0f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_yazirushi->GetTextureWidth());
	textureHeight = static_cast<float>(spr_yazirushi->GetTextureHeight());

	if (sentaku)
	{
		// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
		spr_yazirushi->Render(
			dc,
			screenWidth / 3.5f, screenHeight / 3.2f,
			textureWidth / 2.5f, textureHeight / 2.5f,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
	}
	else
	{
		// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
		spr_yazirushi->Render(
			dc,
			screenWidth / 3.5f, screenHeight / 2.6f,
			textureWidth / 2.5f, textureHeight / 2.5f,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
	}

	textureWidth = static_cast<float>(spr_retry->GetTextureWidth());
	textureHeight = static_cast<float>(spr_retry->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_retry->Render(
		dc,
		screenWidth / 2.5f, screenHeight / 3.0f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);

	textureWidth = static_cast<float>(spr_retire->GetTextureWidth());
	textureHeight = static_cast<float>(spr_retire->GetTextureHeight());

	// UI�̈ʒu�����A���l�ǂ��Y��ɂ���
	spr_retire->Render(
		dc,
		screenWidth / 2.5f, screenHeight / 2.45f,
		textureWidth / 0.6f, textureHeight / 0.6f,
		0, 0,
		textureWidth, textureHeight,
		0,
		1, 1, 1, 1
	);
}