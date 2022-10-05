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
    enum class StageName
    {
        ExampleStage,
    };

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

    int GetStageCount() const { return static_cast<int>(stages.size()); }

    Stage* GetStage(int index) { return stages.at(index); }

    int GetNowStage() { return nowStage; }
    void SetNowStage(const int index) { nowStage = index; }

private:
    std::vector<Stage*> stages;

    int nowStage = 0;
};