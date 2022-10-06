#include "EnemySlime.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "PlayerManager.h"
#include "StateDerived.h"

// コンストラクタ
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	radius = 0.5f;
	height = 1.0f;

	// TODO 03_06 StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
	stateMachine = new StateMachine();
	// ステートマシンに1層目のステートを登録(BattleStateも同様の方法で各自追加してください)
	stateMachine->RegisterState(new SearchState(this));
	stateMachine->RegisterState(new BattleState(this));
	// 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Search), new WanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Search), new IdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Battle), new PursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemySlime::State::Battle), new AttackState(this));
	// ステートをセット
	stateMachine->SetState(static_cast<int>(State::Search));
}

// デストラクタ
EnemySlime::~EnemySlime()
{
	delete model;
}

void EnemySlime::Update(float elapsedTime)
{
	// TODO 02_05 switch文によるFSMをStateMachineクラスを使用したものに書き換える
	stateMachine->Update(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

// 死亡した時に呼ばれる
void EnemySlime::OnDead()
{
	// 死亡
	Destroy();
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}


void EnemySlime::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}

// 縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

// ターゲット位置をランダム設定
void EnemySlime::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemySlime::SearchPlayer()
{

	// プレイヤーとの高低差を考慮して3Dで距離判定をする
	PlayerManager& playerManager = PlayerManager::Instance();
	const DirectX::XMFLOAT3& playerPosition = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// 単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;

		// 方向ベクトル化
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// デバッグエネミー情報表示
void EnemySlime::DrawDebugGUI()
{

	// TODO 03_07 デバッグ文字列表示の変更
	std::string str = "";
	std::string subStr = "";

	// TODO 05 デバッグ表示用に各ステート名をstrに追加しなさい
	// TODO 06＿02 switch文によるFSMをStateMachineクラスを使用したものに書き換える
	switch (static_cast<State>(stateMachine->GetStateIndex())) {
	case State::Search:
		str = "Search";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Search::Wander))
		{
			subStr = "Wander";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Search::Idle))
		{
			subStr = "Idle";
		}
		break;
	case State::Battle:
		str = "Battle";
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Battle::Pursuit))
		{
			subStr = "Pursuit";
		}
		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemySlime::Battle::Attack))
		{
			subStr = "Attack";
		}
		break;
	}


	//トランスフォーム
	if (ImGui::CollapsingHeader("EnemySlime", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 位置
		ImGui::InputFloat3("Position", &position.x);
		// 回転
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// スケール
		ImGui::InputFloat3("Scale", &scale.x);

		ImGui::Text(u8"State　%s", str.c_str());
		ImGui::Text(u8"SubState　%s", subStr.c_str());
	}
}