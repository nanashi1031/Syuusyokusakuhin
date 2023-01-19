#include "Insect.h"
#include "PlayerManager.h"
#include "Mathf.h"
#include "InsectDerived.h"
#include "Collision.h"
#include "StageManager.h"
#include <imgui.h>

Insect::Insect()
{
    model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    scale.x = scale.y = scale.z = 1.0f;

	position.x = -30.0f;
	position.y = 100.0f;

    radius = 0.5f;
    height = 2.0f;
    moveSpeed = 5.0f;

    stateMachine = new StateMachine();
#pragma region ステート登録
    stateMachine->RegisterState(new InsectState::IdleState(this));
    stateMachine->RegisterState(new InsectState::PursuitState(this));
    stateMachine->RegisterState(new InsectState::FlyingState(this));
    stateMachine->RegisterState(new InsectState::ReturnState(this));
#pragma endregion
    stateMachine->SetState(State::Idle);
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

    model->UpdateTransform(transform);

    // 経過フレーム
    float elapsedFrame = elapsedTime * 60.0f;

    // 垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);
    // 垂直移動更新処理
    UpdateVerticalMove(elapsedTime);
    // 水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);
    // 水平移動処理更新
    UpdateHorizontalMove(elapsedTime);

    // 速力処理更新
    //UpdateVelocity(elapsedTime);

    stateMachine->Update(elapsedTime);
    //PlayerWeaponTracking(elapsedTime);
}

void Insect::Render(RenderContext rc, ModelShader* shader)
{
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
            ImGui::DragFloat("moveSpeed", &moveSpeed, 0.1f);
            ImGui::DragFloat3("SwordPostion", &weaponPosition.x, 0.1f);
        }
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
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

void Insect::PlayerWeaponTracking(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();

    Model::Node* playerWeponFront = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandIndex1");
    Model::Node* playerWeponBack = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandMiddle1");

    DirectX::XMFLOAT3 playerWeponFrontPosition;
    playerWeponFrontPosition.x = playerWeponFront->worldTransform._41;
    playerWeponFrontPosition.y = playerWeponFront->worldTransform._42;
    playerWeponFrontPosition.z = playerWeponFront->worldTransform._43;

    DirectX::XMFLOAT3 playerWeponBackPosition;
    playerWeponBackPosition.x = playerWeponBack->worldTransform._41;
    playerWeponBackPosition.y = playerWeponBack->worldTransform._42;
    playerWeponBackPosition.z = playerWeponBack->worldTransform._43;

    DirectX::XMFLOAT3 weaponVector = {};
    weaponVector = Mathf::SubtractFloat3(playerWeponFrontPosition, playerWeponBackPosition);

    DirectX::XMVECTOR playerV;
    playerV = DirectX::XMLoadFloat3(&weaponVector);
    playerV = DirectX::XMVector3Normalize(playerV);
    DirectX::XMFLOAT3 insectPosition = {};
    DirectX::XMStoreFloat3(&weaponVector, playerV);

    insectPosition = Mathf::MultiplyFloat3(weaponVector, DirectX::XMFLOAT3(1, 1, 1));

    insectPosition = Mathf::MultiplyFloat3(insectPosition, position);

    weaponPosition = playerWeponFrontPosition;
    position = insectPosition;
    position = playerWeponFrontPosition;
}

void Insect::UpdateVerticalVelocity(float elapsedFrame)
{

}

void Insect::UpdateVerticalMove(float elapsedTime)
{
 //   slopeRate = 0.0f;

 //   // 落下中
 //   if (velocity.y < 0.0f)
 //   {
 //       // レイの開始位置は足元より少し上
 //       DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset , position.z };
 //       // レイの終了位置は移動後の位置
 //       DirectX::XMFLOAT3 end = { position.x,position.y + velocity.y , position.z };

 //       // レイキャストによる地面判定
 //       HitResult hit;
 //       StageManager& stageManager = StageManager::Instance();
 //       if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
 //       {
 //           // 地面に接地している
 //           position.y = hit.position.y;

 //           velocity.y = 0.0f;
 //       }
 //   }

	//position.y += velocity.y;
}

void Insect::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsedFrame;

		// 空中にいるときは摩擦力を減らす
		if (!isGround) friction *= airControl;

		// 摩擦による横方向の減速処理
		if (length > friction)
		{
			// 単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// 横方向の速度が摩擦力以下になったので速力を無効化
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ平面の速力を加速する
	if (length <= maxMoveSpeed)
	{
		// 移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsedFrame;
			// 空中にいるときは加速力を減らす
			if (!isGround)acceleration *= airControl;

			// 移動ベクトルによる加速処理
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			// 最大速度制限
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}
		}
	}
	// 移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
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