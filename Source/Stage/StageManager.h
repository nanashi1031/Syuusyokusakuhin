#pragma once

#include <vector>
#include "Stage.h"

// ステージマネージャー
class StageManager
{
    StageManager() {};
    ~StageManager() {};

public:
    // 唯一のインスタンス取得
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