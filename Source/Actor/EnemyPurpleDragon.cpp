#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "EnemyPurpleDragon.h"
#include "JudgmentDerived.h"
#include "EnemyPurpleDragonActionDerived.h"

EnemyPurpleDragon::EnemyPurpleDragon()
{
	model = new Model("Data/Model/Enemy/PurpleDragon/PurpleDragon.mdl");

	scale.x = scale.y = scale.z = 0.01f;

	radius = 3.0f;
	height = 1.5f;

	health = 100.0f;

	// ビヘイビアツリー設定
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

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

void EnemyPurpleDragon::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemyPurpleDragon::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
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
		ImGui::DragFloat("Health", &health);
		//ImGui::EndChild();
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
	SE_Attack->Play(false);
}