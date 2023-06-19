#include "Graphics.h"
#include "Mathf.h"
#include "EnemyPurpleDragon.h"
#include "JudgmentDerived.h"
#include "EnemyPurpleDragonDerived.h"
#include "Input.h"

EnemyPurpleDragon::EnemyPurpleDragon()
{
	model = new Model("Data/Model/Enemy/PurpleDragon/PurpleDragon.mdl");

	scale.x = scale.y = scale.z = 0.02f;

	radius = 3.0f;
	height = 1.5f;

	maxHealth = 15000.0f;
	health = maxHealth;

	CharacterType = Type::Boss;

	PartsRegister();

	stateMachine = new StateMachine();

#pragma region ステート登録
	stateMachine->RegisterState(new EnemyPurpleDragonState::IdleState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::BattleIdleState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::NeglectState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::HowlState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::PursuitState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::WalkState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::RunState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::TakeOffState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::FlyState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::LandingState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::SleepState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::BiteAttackState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::ClawAttackState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::DamagesState(this));
	stateMachine->RegisterState(new EnemyPurpleDragonState::DieState(this));
#pragma endregion

	stateMachine->SetState(State::Landing);

	SE_Attack = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/Attack.wav");
}

EnemyPurpleDragon::~EnemyPurpleDragon()
{
	delete model;
}

void EnemyPurpleDragon::Update(float elapsedTime)
{
	if (!moveStop)
	{
		stateMachine->Update(elapsedTime);

		// 速力処理更新
		UpdateVelocity(elapsedTime);

		// オブジェクト行列更新
		UpdateTransform();

		model->UpdateAnimation(elapsedTime);

		model->RootMotion("Root");
	}

	UpdateInvincibleTime(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemyPurpleDragon::Render(RenderContext rc, ModelShader* shader)
{
	shader->Draw(rc, model);
}

void EnemyPurpleDragon::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius,
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f,
		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	// 部位情報をデバック球描画
	if (partsIndex > 0)
	{
		DirectX::XMFLOAT3 partsPosition =
			GetNodePosition(model->FindNode(GetParts()[partsIndex - 1].name));
		DirectX::XMFLOAT4 color =
			Extract::Instance().ColorConversion(GetParts()[partsIndex - 1].extractColor);
		debugRenderer->DrawSphere(partsPosition, GetParts()[partsIndex - 1].radius + 0.01f,
			DirectX::XMFLOAT4(color));
	}
}

void EnemyPurpleDragon::DrawDebugGUI()
{
	ImVec2 windowPosition = { 10, 10 };
	ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
	ImVec2 windowSize = { 300, 300 };
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
	// ウィンドウの透明度
	float alpha = 0.35f;
	ImGui::SetNextWindowBgAlpha(alpha);
	//ImGui::SetNextTreeNodeOpen();

	if (ImGui::Begin("EnemyPurpleDragon", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
		{
				char* str = {};
				switch (GetStateMachine()->GetStateIndex())
				{
				case static_cast<int>(State::Idle):
					str = "Idle";
					break;
				case static_cast<int>(State::Neglect):
					str = "Neglect";
					break;
				case static_cast<int>(State::Pursuit):
					str = "Pursuit";
					break;
				case static_cast<int>(State::Walk):
					str = "Walk";
					break;
				case static_cast<int>(State::Run):
					str = "Run";
					break;
				case static_cast<int>(State::Fly):
					str = "Fly";
					break;
				case static_cast<int>(State::BiteAttack):
					str = "BiteAttack";
					break;
				case static_cast<int>(State::ClawAttack):
					str = "ClawAttack";
					break;
				case static_cast<int>(State::Damages):
					str = "Damages";
					break;
				case static_cast<int>(State::Die):
					str = "Die";
					break;
				}
				ImGui::Text("NowState %s", str);
				if (ImGui::Button("Stop"))
				{
					moveStop = !moveStop;
				}
		}

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
		}
		ImGui::SliderFloat("Health", &health, 0.0f, maxHealth);
		ImGui::SliderFloat("invincibleTimer", &invincibleTimer, 0.0f, 1.0f);
		//ImGui::EndChild();

		if (ImGui::CollapsingHeader("parts", ImGuiTreeNodeFlags_DefaultOpen))
		{
			char* listbox_items[44];
			listbox_items[0] = "None";
			for (int i = 0; i < GetParts().size(); i++)
			{
				listbox_items[i + 1] = GetParts()[i].name;
			}
			ImGui::ListBox("name", &partsIndex, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
			if (partsIndex > 0)
			{
				int index = partsIndex - 1;
				ImGui::Text("name %s", GetParts()[index].name);
				float radius  = GetParts()[index].radius;
				ImGui::DragFloat("radius", &radius, 0.01f);
				int cameraTargetFlag = GetParts()[index].cameraTargetFlag;
				ImGui::Combo("combo", &cameraTargetFlag, "fales\0true\0\0");
				float defensePower = GetParts()[index].defensePower;
				ImGui::DragFloat("defensePower", &defensePower, 0.1f);
				int extractColor = GetParts()[index].extractColor;
				ImGui::DragInt("ExtractColor", &extractColor, 0, 4);
				SetPartsSpecify(
					index, GetParts()[index].name, radius,
					cameraTargetFlag, defensePower, extractColor);
			}
		}
	}
	ImGui::End();
}

// 縄張り設定
void EnemyPurpleDragon::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void EnemyPurpleDragon::OnDamaged()
{

}

void EnemyPurpleDragon::OnDead()
{
	stateMachine->ChangeState(State::Die);
}

void EnemyPurpleDragon::OnDown()
{

}

void EnemyPurpleDragon::PartsRegister()
{
	SetParts("Jaw1", 1.8f, ExtractColor::Red, 0.0f, true);
	SetParts("Neck3", 1.8f, ExtractColor::Red, 1.0f);
	SetParts("Neck1", 1.8f, ExtractColor::Red, 1.0f);
	SetParts("Hand_L", 1.2f, ExtractColor::White, 3.0f, true);
	SetParts("LowerArm_L", 1.2f, ExtractColor::White, 5.0f);
	SetParts("Hand_L_1", 1.2f, ExtractColor::White, 3.0f, true);
	SetParts("LowerArm_L_1", 1.2f, ExtractColor::White, 5.0f);
	SetParts("Feet_L", 1.2f, ExtractColor::Orange, 4.0f, true);
	SetParts("LowerLeg_L", 1.2f, ExtractColor::Orange, 6.0f);
	SetParts("Feet_L_1", 1.2f, ExtractColor::Orange, 4.0f, true);
	SetParts("LowerLeg_L_1", 1.2f, ExtractColor::Orange, 6.0f);
	SetParts("Wing1_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Wing2_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Wing3_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail12_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail2_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail3_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail5_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail6_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail7_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail9_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail10_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail11_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Wing1_L1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Wing2_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Wing3_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingClaw4_L", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingClaw4_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail12_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail2_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail3_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail5_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail6_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail7_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail9_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail10_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("WingDetail11_L_1", 1.8f, ExtractColor::Orange, 2.0f);
	SetParts("Root", 1.5f, ExtractColor::Heal, 0.0f);
	SetParts("Tail1", 1.0f, ExtractColor::Heal, 1.0f);
	SetParts("Tail2", 1.0f, ExtractColor::Heal, 1.0f);
	SetParts("Tail3", 1.0f, ExtractColor::Heal, 1.0f);
	SetParts("Tail4", 1.0f, ExtractColor::Heal, 1.0f, true);
	SetParts("Tail5", 1.0f, ExtractColor::Heal, 1.0f);
}