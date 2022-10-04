#include "Character.h"
#include "Collision.h"
#include "StageManager.h"

void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (vx == 0.0f && vz == 0.0f)
        return;

    //進行ベクトルを単位ベクトル化
    float v = sqrt(pow(vx, 2.0f) + pow(vz, 2.0f));
    vx /= v;
    vz /= v;

    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //左右判定を行うために2つの単位ベクトルの外積を計算する
    float cross = (vx * frontZ) - (vz * frontX);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (vx * frontX) + (vz * frontZ);

    //内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    //小さいほどに1.0に近づくという性質を利用して回転速度を調整する
    float rot = 1.0f - dot;

    //2Dの外積値が正の場合か負の場合によって左右判定が行える
    //左右判定を行うことによって左右回転を選択する
    if (cross < 0.0f)
    {
        angle.y -= speed * rot;
    }
    else
    {
        angle.y += speed * rot;
    }

}

void Character::UpdateVelocity(float elapsedTime)
{
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
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    velocity.y += gravity * elapsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    float my = velocity.y * elapsedTime;
    slopeRate = 0.0f;

    // 落下中
    if (my < 0.0f)
    {
        // レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset , position.z };
        // レイの終了位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my , position.z };

        // レイキャストによる地面判定
        HitResult hit;
        StageManager& stageManager = StageManager::Instance();
        if (stageManager.GetStage(static_cast<int>(StageManager::StageName::ExampleStage)))
        {
            // 地面に接地している
            position.y = hit.collisionPosition.y;

            // 傾斜率の計算
            float normalLengthXZ = sqrtf(hit.collisionNormal.x * hit.collisionNormal.x + hit.collisionNormal.z * hit.collisionNormal.z);
            slopeRate = 1.0f - (hit.collisionNormal.y / (normalLengthXZ + hit.collisionNormal.y));

            // 着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}

// 水平速力処理更新
void Character::UpdateHorizontalVelocity(float elapsedFrame)
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
// 水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	// 水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// レイとの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x , position.y + stepOffset , position.z };
		DirectX::XMFLOAT3 end = { position.x + mx , position.y + stepOffset , position.z + mz };

		// レイキャストによる壁判定
		HitResult hit;
		if (Stage::Instance().RayCast(start, end, hit))
		{
			// 壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// 壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// 補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			// 壁ずり方向へレイキャスト
			HitResult hit2;
			if (!Stage::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				// 壁ずり方向で壁に当たらなかったら補正位置に移動
				position.x = collectPosition.x;
				position.z = collectPosition.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
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