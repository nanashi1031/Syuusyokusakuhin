#include "Insect.h"
#include "PlayerManager.h"
#include "Mathf.h"
#include "InsectDerived.h"
#include "Collision.h"
#include "Camera.h"
#include "StageManager.h"
#include "LightManager.h"
#include <imgui.h>

Insect::Insect()
{
    model = std::make_shared<Model>("Data/Model/Insect/Butterfly.mdl");
    scale.x = scale.y = scale.z = 0.05f;

    radius = 0.5f;
    height = 2.0f;
    moveSpeed = 30.0f;
	turnSpeed = 10.0f;

	recoveryAmount = 30.0f;

	model->PlayAnimation(0, true);

	stateMachine = new StateMachine();
#pragma region ステート登録
    stateMachine->RegisterState(new InsectState::IdleState(this));
    stateMachine->RegisterState(new InsectState::PursuitState(this));
    stateMachine->RegisterState(new InsectState::FlyingState(this));
    stateMachine->RegisterState(new InsectState::ReturnState(this));
#pragma endregion
    stateMachine->SetState(State::Pursuit);

	ParticleRed = std::make_unique<Effect>("Data/Effect/InsectParticleRed.efk");
	ParticleWhite = std::make_unique<Effect>("Data/Effect/InsectParticleWhite.efk");
}

Insect::~Insect()
{

}

void Insect::Initialize()
{

}

void Insect::Update(float elapsedTime)
{
    beforPosition = position;

    UpdateTransform();

	model->UpdateAnimation(elapsedTime);
    model->UpdateTransform(transform);

    // 経過フレーム
    float elapsedFrame = elapsedTime * 60.0f;

    // 垂直移動更新処理
    UpdateVerticalMove(elapsedTime);
    // 水平移動処理更新
    UpdateHorizontalMove(elapsedTime);

	Turn(elapsedTime, velocity.x, velocity.z, turnSpeed);

    stateMachine->Update(elapsedTime);

	UpdateLight();

	// エフェクトが完成したら解除
	// UpdateExtractEffect();
}

void Insect::Render(RenderContext rc, ModelShader* shader)
{
	if (stateMachine->GetStateIndex() != static_cast<int>(Insect::State::Pursuit))
		shader->Draw(rc, model);
}

void Insect::DrawDebugPrimitive()
{

}

void Insect::DrawDebugGUI()
{
    ImVec2 windowPosition = { 10, 10 };
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
    ImVec2 windowSize = { 300, 300 };
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);
    //ImGui::SetNextTreeNodeOpen();

    if (ImGui::Begin("Insect", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Postion", &position.x, 0.1f);
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::DragFloat3("Angle", &a.x, 0.1f, 0, 360);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);

			ImGui::DragFloat3("Scale", &scale.x, 0.0005f, 0, 1000);
            ImGui::DragFloat("MoveSpeed", &moveSpeed, 0.1f);
			int color = extractColor;
			ImGui::InputInt("extractColor", &color);
			extractColor = color;
        }
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
			if (ImGui::Button("Pursuit"))
			{
				stateMachine->ChangeState(State::Pursuit);
			}
            std::string str = "";
            int nowState = stateMachine->GetStateIndex();
            if (nowState == static_cast<int>(Insect::State::Idle))
            {
                str = "Idle";
            }
            if (nowState == static_cast<int>(Insect::State::Pursuit))
            {
                str = "Pursuit";
            }
            if (nowState == static_cast<int>(Insect::State::Flying))
            {
                str = "Flying";
            }
            if (nowState == static_cast<int>(Insect::State::Return))
            {
                str = "Return";
            }
            ImGui::Text(u8"State　%s", str.c_str());
        }
    }

    ImGui::End();
}

void Insect::UpdateVerticalMove(float elapsedTime)
{
	float my = velocity.y * elapsedTime;
	if (my < 0.0f)
	{
		DirectX::XMVECTOR Velocity =
			DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);

		DirectX::XMFLOAT3 start = { position.x, position.y, position.z };
		DirectX::XMFLOAT3 end = {
			position.x + DirectX::XMVectorGetX(Velocity),
			position.y + my + DirectX::XMVectorGetZ(Velocity),
			position.z + DirectX::XMVectorGetZ(Velocity) };

		// レイキャストによる床判定
		HitResult hit;
		StageManager& stageManager = StageManager::Instance();
		if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
		{
			position.y = hit.position.y;
			velocity.y = 0.0f;
		}
		else
		{
			position.y += my;
		}
	}
	else
	{
		position.y += my;
	}
}

void Insect::UpdateHorizontalMove(float elapsedTime)
{
	// 水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);

		// レイとの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx + DirectX::XMVectorGetX(Velocity), position.y + stepOffset, position.z + mz + DirectX::XMVectorGetZ(Velocity) };

		// レイキャストによる壁判定
		HitResult hit;
		StageManager& stageManager = StageManager::Instance();
		if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
		{
			// 壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			End = DirectX::XMVectorSubtract(End, Velocity);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// 壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// 補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			collectPosition.y = position.y;
			hit.position.y = position.y;

			// 壁ずり方向へレイキャスト
			HitResult hit2;
			if (!stageManager.GetStage(stageManager.GetNowStage())->RayCast(hit.position, collectPosition, hit2))
			{
				hit2.position.y = position.y;
				// 壁ずり方向で壁に当たらなかったら補正位置に移動
#if 0
				position.x = collectPosition.x;
				position.z = collectPosition.z;
#endif
				hit.normal.y = 0;
				Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit.normal));
				Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit.position));
				Dot = DirectX::XMVector3Dot(Vec, Normal);
				DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&collectPosition), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
				float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&position)), Normal));

				// positionをスタートにすることでその場から動かない(はずだったのに)
				//start.y = position.y;
				//position = start;

				DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorScale(Normal, radius + dot)));
			}
			else
			{
				//position.x = hit2.position.x;
				//position.z = hit2.position.z;

				hit2.position.y = position.y;
				hit2.normal.y = 0;
				Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit2.normal));
				Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit2.position));
				Dot = DirectX::XMVector3Dot(Vec, Normal);
				DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit2.position), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
			}
		}
		else
		{
			// 移動
			position.x += mx;
			position.z += mz;
		}

	}
}

void Insect::UpdateLight()
{
	if (lightIndex >= 0)
	{
		DirectX::XMFLOAT3 length =
			Mathf::CalculateLength(Camera::Instance().GetEye(), position);
		length = Mathf::MultiplyFloat3Float(length, radius + radius);
		DirectX::XMFLOAT3 lightPosition =
		{ position.x + length.x, position.y + length.y, position.z + length.z };
		LightManager::Instane().GetLight(lightIndex - 1)->SetPosition(lightPosition);
	}
}

void Insect::UpdateExtractEffect()
{
	int effectIndex = 0;

	switch (this->GetExtractColor())
	{
	case ExtractColor::Red:
		ParticleRed->Play(this->GetPosition(), 0.2f);
		break;
	case ExtractColor::White:
		ParticleWhite->Play(this->GetPosition(), 0.2f);
		break;
	case ExtractColor::Orange:
		ParticleRed->Play(this->GetPosition(), 0.2f);
		break;
	case ExtractColor::Heal:
		ParticleRed->Play(this->GetPosition(), 0.2f);
		break;
	case ExtractColor::None:
		break;
	}
}