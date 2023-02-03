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
    model = new Model("Data/Model/Insect/Butterfly.mdl");
    scale.x = scale.y = scale.z = 0.05f;

    radius = 0.5f;
    height = 2.0f;
    moveSpeed = 30.0f;
	turnSpeed = 10.0f;

	model->PlayAnimation(0, true);

    stateMachine = new StateMachine();
#pragma region �X�e�[�g�o�^
    stateMachine->RegisterState(new InsectState::IdleState(this));
    stateMachine->RegisterState(new InsectState::PursuitState(this));
    stateMachine->RegisterState(new InsectState::FlyingState(this));
    stateMachine->RegisterState(new InsectState::ReturnState(this));
#pragma endregion
    stateMachine->SetState(State::Pursuit);
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

    // �o�߃t���[��
    float elapsedFrame = elapsedTime * 60.0f;

	//UpdateVelocitys(elapsedFrame);
	//UpdateMoves(elapsedFrame);

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);
    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);
    // �������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);
    // �����ړ������X�V
    UpdateHorizontalMove(elapsedTime);

	Turn(elapsedTime, velocity.x, velocity.z, turnSpeed);

    stateMachine->Update(elapsedTime);

	UpdateLight();
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
    // �E�B���h�E�̓����x
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
            ImGui::DragFloat3("SwordPostion", &weaponPosition.x, 0.1f);
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
            ImGui::Text(u8"State�@%s", str.c_str());
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
	//float length = sqrtf(velocity.y * velocity.y);
	//if (length != 0.0f)
	//{
	//	float friction = this->friction * elapsedFrame;

	//	if (length > friction)
	//	{
	//		float vy = velocity.y / length;

	//		velocity.y -= vy * friction;
	//	}
	//	else
	//	{
	//		velocity.y = 0.0f;
	//	}
	//}
	//if (length <= maxMoveSpeed)
	//{
	//	float moveVecLength = sqrtf(moveVecY * moveVecY);
	//	if (moveVecLength > 0.0f)
	//	{
	//		float acceleration = this->acceleration * elapsedFrame;

	//		velocity.y += moveVecY * acceleration;

	//		// �ő呬�x����
	//		float length = sqrtf(velocity.y * velocity.y);
	//		if (length > maxMoveSpeed)
	//		{
	//			float vy = velocity.y / length;

	//			velocity.y = vy * maxMoveSpeed;
	//		}
	//		if (isGround && slopeRate > 0.0f)
	//		{
	//			velocity.y -= length * slopeRate * elapsedFrame;
	//		}
	//	}
	//}
	//moveVecY = 0.0f;
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

		// ���C�L���X�g�ɂ�鏰����
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

void Insect::UpdateHorizontalVelocity(float elapsedFrame)
{
	//// XZ���ʂ̑��͂���������
	//float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	//if (length > 0.0f)
	//{
	//	// ���C��
	//	float friction = this->friction * elapsedFrame;

	//	// �󒆂ɂ���Ƃ��͖��C�͂����炷
	//	if (!isGround) friction *= airControl;

	//	// ���C�ɂ�鉡�����̌�������
	//	if (length > friction)
	//	{
	//		// �P�ʃx�N�g����
	//		float vx = velocity.x / length;
	//		float vz = velocity.z / length;

	//		velocity.x -= vx * friction;
	//		velocity.z -= vz * friction;
	//	}
	//	// �������̑��x�����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
	//	else
	//	{
	//		velocity.x = 0.0f;
	//		velocity.z = 0.0f;
	//	}
	//}
	//// XZ���ʂ̑��͂���������
	//if (length <= maxMoveSpeed)
	//{
	//	// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
	//	float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
	//	if (moveVecLength > 0.0f)
	//	{
	//		// ������
	//		float acceleration = this->acceleration * elapsedFrame;
	//		// �󒆂ɂ���Ƃ��͉����͂����炷
	//		if (!isGround)acceleration *= airControl;

	//		// �ړ��x�N�g���ɂ���������
	//		velocity.x += moveVecX * acceleration;
	//		velocity.z += moveVecZ * acceleration;

	//		// �ő呬�x����
	//		float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	//		if (length > maxMoveSpeed)
	//		{
	//			float vx = velocity.x / length;
	//			float vz = velocity.z / length;

	//			velocity.x = vx * maxMoveSpeed;
	//			velocity.z = vz * maxMoveSpeed;
	//		}
	//		if (isGround && slopeRate > 0.0f)
	//		{
	//			velocity.y -= length * slopeRate * elapsedFrame;
	//		}
	//	}
	//}
	//// �ړ��x�N�g�������Z�b�g
	//moveVecX = 0.0f;
	//moveVecZ = 0.0f;
}

void Insect::UpdateHorizontalMove(float elapsedTime)
{
	// �������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);

		// ���C�Ƃ̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx + DirectX::XMVectorGetX(Velocity), position.y + stepOffset, position.z + mz + DirectX::XMVectorGetZ(Velocity) };

		// ���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		StageManager& stageManager = StageManager::Instance();
		if (stageManager.GetStage(stageManager.GetNowStage())->RayCast(start, end, hit))
		{
			// �ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			End = DirectX::XMVectorSubtract(End, Velocity);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// �ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			collectPosition.y = position.y;
			hit.position.y = position.y;

			// �ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!stageManager.GetStage(stageManager.GetNowStage())->RayCast(hit.position, collectPosition, hit2))
			{
				hit2.position.y = position.y;
				// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
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

				// position���X�^�[�g�ɂ��邱�Ƃł��̏ꂩ�瓮���Ȃ�(�͂��������̂�)
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
			// �ړ�
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