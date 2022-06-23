#include "Character.h"

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