#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include "StageMain.h"
#include "Collision.h"

// ステージマネージャー
class StageManager
{
    StageManager() {};
    ~StageManager() {};

public:
    enum class StageName
    {
        ExampleStage,
    };

    // 唯一のインスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    void Update(float elapsedTime);

    void Render(RenderContext rc, ModelShader* shader);

    void DrawDebugGUI();

    void Register(std::unique_ptr<StageMain> stage)
    {
        stages.emplace_back(stage.get());
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