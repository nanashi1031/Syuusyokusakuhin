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
        if (stageManager.GetStage(static_cast<int>(StageManager::StageName::ExampleStage)));
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