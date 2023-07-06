#include "Graphics.h"
#include "SceneClear.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Input.h"
#include "StageManager.h"
#include "StageMain.h"
#include "LightManager.h"
#include "Camera.h"
#include "CameraController.h"
#include "Scene\SceneTitle.h"

//������
void SceneClear::Initialize()
{
	//�X�v���C�g������
	spr_clear = std::make_unique<Sprite>("Data/Sprite/Scene/Clear.png");

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
		0.1f,
		1000.0f
	);

	{
		mainDirectionalLight = std::make_unique<Light>(LightType::Directional);
		mainDirectionalLight->SetPosition({ 0.70f, 0.60f, 0.30f });
		mainDirectionalLight->SetDirection({ 1, -1, -1 });
		mainDirectionalLight->SetColor({ 0, 0, 0, 1 });
		LightManager::Instane().Register(mainDirectionalLight.get());
	}

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
}

//�I����
void SceneClear::Finalize()
{
	LightManager::Instane().Clear();
}

//�X�V����
void SceneClear::Update(float elapsedTime)
{
	DirectX::XMFLOAT3 target = { 0, 0, 0 };
	CameraController::Instance().SetTarget(target);

	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����{�^�����������烍�[�f�B���O�V�[��������ŃQ�[���V�[���֐؂�ւ�
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}
#if defined(_DEBUG)
#endif
//�`�揈��
void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };     //RGBA(0.0~1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	RenderContext rc;
	rc.deviceContext = dc;
	{
		// ���C�g�̏����l�ߍ���
		LightManager::Instane().PushRenderContext(rc);

		// �J����
		Camera& camera = Camera::Instance();
		rc.viewPosition.x = camera.GetEye().x;
		rc.viewPosition.y = camera.GetEye().y;
		rc.viewPosition.z = camera.GetEye().z;
		rc.viewPosition.w = 1;
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();

		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
	}
	//2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(spr_clear->GetTextureWidth());
		float textureHeight = static_cast<float>(spr_clear->GetTextureHeight());
		//�^�C�g���X�v���C�g�`��
		spr_clear->Render(dc,
			screenWidth / 3.0f, screenHeight / 3.0f,
			textureWidth / 0.6f, textureHeight / 0.6f,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1);
	}
}

// 3D��Ԃ̕`��
void SceneClear::Render3DScene()
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

		modelShader->End(rc);
	}

	// 3D�f�o�b�O�`��
	{
#ifdef _DEBUG
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
		LightManager::Instane().DrawDebugPrimitive();
#endif
	}
}