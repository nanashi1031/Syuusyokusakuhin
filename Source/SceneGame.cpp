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
	// �X�e�[�W������
	// �X�}�[�g�|�C���^�̂ق�������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// �v���C���[
	PlayerManager& playerManager = PlayerManager::Instance();
	Player* player = new Player;
	playerManager.Register(player);

	EnemyManager& enemyManager = EnemyManager::Instance();
	//�G�l�~�[
	for (int i = 0; i < 2; i++)
	{
		EnemyBoss* boss = new EnemyBoss();
		boss->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 10.0f));
		enemyManager.Register(boss);
	}

	EnemySlime* slime = new EnemySlime;
	slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 10.0f));
	enemyManager.Register(slime);

	// �J����
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

	// �X�v���C�g
	targetRing = std::make_unique<Sprite>("Data/Sprite/Ring.png");
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

	//�G�l�~�[
	EnemyManager::Instance().Clear();
}

void SceneGame::Update(float elapsedTime)
{
	StageManager::Instance().Update(elapsedTime);

	cameraController->Update(elapsedTime);
	DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetplayerOneIndex())->GetPosition();
	// ��������ɐݒ�
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

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J����
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		StageManager::Instance().Render(dc, shader);

		PlayerManager::Instance().Render(dc, shader);

		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D�f�o�b�O�`��
	{
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		PlayerManager::Instance().DrawDebugPrimitive();

		EnemyManager::Instance().DrawDebugPrimitive();
	}

	// 2D�X�v���C�g�`��
	{
		if (cameraController->GetTagetIndex() >= 0)
			RenderLockOn(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
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