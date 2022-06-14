#pragma once

#include <vector>
#include "Stage.h"

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

private:
    std::vector<Stage*> stages;
};