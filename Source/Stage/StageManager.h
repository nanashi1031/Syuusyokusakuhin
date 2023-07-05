#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include "StageMain.h"
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

    void Render(RenderContext rc, ModelShader* shader);

    void DrawDebugGUI();

    void Register(Stage* stage)
    {
        stages.emplace_back(stage);
    }

    void Clear();

    int GetStageCount() const { return static_cast<int>(stages.size()); }

    Stage* GetStage(int index) { return stages.at(index); }

    int GetNowStage() { return nowStage; }
    void SetNowStage(const int index) { nowStage = index; }

private:
    std::vector<Stage*> stages;

    int nowStage = 0;
};