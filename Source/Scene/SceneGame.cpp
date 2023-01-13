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

//	�V���h�E�}�b�v�̃T�C�Y
static	const	UINT	SHADOWMAP_SIZE = 2048;

void SceneGame::Initialize()
{
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
		cameraController = new CameraController();
	}

	// �X�v���C�g
	targetRing = std::make_unique<Sprite>("Data/Sprite/Ring.png");

	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1, -1, -1 });
		LightManager::Instane().Register(mainDirectionalLight);
	}
	// �_����
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(4, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instane().Register(light);
	}

	// �X�|�b�g���C�g
	{
		Light* light = new Light(LightType::Spot);
		light->SetPosition(DirectX::XMFLOAT3(-10, 6, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		light->SetRange(10.0f);
		LightManager::Instane().Register(light);
	}

	// �K�E�X�u���[
	{
		texture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");

		gaussianBlurSprite = std::make_unique<Sprite>();
		gaussianBlurSprite->SetShaderResourceView(
			texture->GetShaderResourceView(),
			texture->GetWidth(), texture->GetHeight());

	}

	// �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
	{
		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// �X�J�C�{�b�N�X
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
	// �I����

	StageManager::Instance().Clear();

	//�J�����R���g���[���[
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�v���C���[
	PlayerManager::Instance().Clear();

	// ��
	InsectManager::Instance().Clear();

	//�G�l�~�[
	EnemyManager::Instance().Clear();
}

void SceneGame::Update(float elapsedTime)
{
	StageManager::Instance().Update(elapsedTime);

	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
	// ��������ɐݒ�
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

	// �J����
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//RenderShadowmap();
	Render3DScene();

	// 3D���f���`��
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

	// 2D�X�v���C�g�`��
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

	// 2D�f�o�b�OGUI�`��
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

	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numVieports = 1;
	dc->RSGetViewports(&numVieports, &viewport);
	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// �G�l�~�[�̓���̃��[���h���W
	DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	worldPosition.y += enemy->GetHeight();
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
}

// 3D��Ԃ̕`��
void SceneGame:: Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// �X�J�C�{�b�N�X�̕`��
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
	}

	// 3D���f���`��
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