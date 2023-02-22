#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "EnemyPurpleDragon.h"
#include "JudgmentDerived.h"
#include "EnemyPurpleDragonActionDerived.h"
#include "EnemyPurpleDragonDerived.h"

EnemyPurpleDragon::EnemyPurpleDragon()
{
	model = new Model("Data/Model/Enemy/PurpleDragon/PurpleDragon.mdl");

	scale.x = scale.y = scale.z = 0.02f;

	radius = 3.0f;
	height = 1.5f;

	maxHealth = 15000.0f;
	health = maxHealth;

	CharacterType = 1;

	PartsRegister();

	// ビヘイビアツリー設定
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

#pragma region ビヘイビアツリー登録
	aiTree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
	aiTree->AddNode("Root", "Escape", 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Scout", 5, BehaviorTree::SelectRule::Priority, nullptr, nullptr);


	aiTree->AddNode("Scout", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	aiTree->AddNode("Scout", "Idle", 2, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));

	aiTree->AddNode("Escape", "Leave", 6, BehaviorTree::SelectRule::Non, nullptr, new LeaveAction(this));
	aiTree->AddNode("Escape", "Recover", 7, BehaviorTree::SelectRule::Non, nullptr, new RecoverAction(this));

	aiTree->AddNode("Battle", "Attack", 8, BehaviorTree::SelectRule::Random, new AttackJudgment(this), nullptr);
	aiTree->AddNode("Attack", "Normal", 9, BehaviorTree::SelectRule::Non, nullptr, new AttackAction(this));
	aiTree->AddNode("Attack", "Skill", 8, BehaviorTree::SelectRule::Non, new SkillShotJudgment(this), new SkillAction(this));
	aiTree->AddNode("Battle", "Pursuit", 10, BehaviorTree::SelectRule::Non, nullptr, new PursuitAction(this));

	aiTree->AddNode("Battle", "Death", 11, BehaviorTree::SelectRule::Non, nullptr, new DeathAction(this));
#pragma endregion

#pragma region ステート登録
	//stateMachine->RegisterState(new EnemyPurpleDragonState::IdleState(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::NeglectState(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::PursuitState(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::WalkState(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::RunState(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::AttackCombo1State(this));
	//stateMachine->RegisterState(new EnemyPurpleDragonState::DieState(this));
#pragma endregion

	model->PlayAnimation(EnemyPurpleDragonAnimation::Sleep, true);

	SE_Attack = Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/Attack.wav");
}

EnemyPurpleDragon::~EnemyPurpleDragon()
{
	delete model;
}

void EnemyPurpleDragon::Update(float elapsedTime)
{
	// 現在実行されているノードが無ければ
	if (activeNode == nullptr)
	{
		// 次に実行するノードを推論する。
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	}
	// 現在実行するノードがあれば
	if (activeNode != nullptr)
	{
		// ビヘイビアツリーからノードを実行。
		activeNode = aiTree->Run(activeNode, behaviorData, elapsedTime);
	}

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	UpdateInvincibleTime(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	model->RootMotion("Root");

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
		//ImGui::EndChild();
		if (ImGui::CollapsingHeader("parts", ImGuiTreeNodeFlags_DefaultOpen))
		{
			char* listbox_items[41];
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
	//SE_Attack->Play(false);
}

void EnemyPurpleDragon::PartsRegister()
{
	SetParts("Jaw1", 1.0f, ExtractColor::Red, true);
	SetParts("Neck3", 1.0f, ExtractColor::Red);
	SetParts("Neck1", 1.0f, ExtractColor::Red);
	SetParts("Hand_L", 1.0f, ExtractColor::White, true);
	SetParts("LowerArm_L", 1.0f, ExtractColor::White);
	SetParts("Hand_L_1", 1.0f, ExtractColor::White, true);
	SetParts("LowerArm_L_1", 1.0f, ExtractColor::White);
	SetParts("Feet_L", 1.0f, ExtractColor::Orange, true);
	SetParts("LowerLeg_L", 1.0f, ExtractColor::Orange);
	SetParts("Feet_L_1", 1.0f, ExtractColor::Orange, true);
	SetParts("LowerLeg_L_1", 1.0f, ExtractColor::Orange);
	SetParts("Wing1_L", 1.0f, ExtractColor::Orange);
	SetParts("Wing2_L", 1.0f, ExtractColor::Orange);
	SetParts("Wing3_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail12_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail2_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail3_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail5_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail6_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail7_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail9_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail10_L", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail11_L", 1.0f, ExtractColor::Orange);
	SetParts("Wing1_L1", 1.0f, ExtractColor::Orange);
	SetParts("Wing2_L_1", 1.0f, ExtractColor::Orange);
	SetParts("Wing3_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail12_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail2_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail3_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail5_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail6_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail7_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail9_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail10_L_1", 1.0f, ExtractColor::Orange);
	SetParts("WingDetail11_L_1", 1.0f, ExtractColor::Orange);
	SetParts("Tail1", 1.0f, ExtractColor::Heal);
	SetParts("Tail2", 1.0f, ExtractColor::Heal);
	SetParts("Tail3", 1.0f, ExtractColor::Heal);
	SetParts("Tail4", 1.0f, ExtractColor::Heal, true);
	SetParts("Tail5", 1.0f, ExtractColor::Heal);
}