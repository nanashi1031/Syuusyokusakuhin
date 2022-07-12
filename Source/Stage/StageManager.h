#pragma once

#include <vector>
#include "Stage.h"
#include "Collision.h"

// �X�e�[�W�}�l�[�W���[
class StageManager
{
    StageManager() {};
    ~StageManager() {};

public:
    // �B��̃C���X�^���X�擾
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void Register(Stage* stage);

    void Clear();

    bool RayaCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

private:
    std::vector<Stage*> stages;
};