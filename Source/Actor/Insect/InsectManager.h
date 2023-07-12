#pragma once

#include <vector>
#include "Insect.h"

//インセクトマネージャー
class InsectManager
{
private:
    InsectManager() {}
    ~InsectManager() {}

public:
    static InsectManager& Instance()
    {
        static InsectManager instance;
        return instance;
    }

    void Initialize();
    void Clear();

    void Update(float elapsedTime);
    void Render(RenderContext rc, ModelShader* shader);

    void Register(Insect* enemy);

    void DrawDebugPrimitive();

    void DrawDebugGUI();

    int GetInsectCount() const { return static_cast<int>(insects.size()); }

    Insect* GetInsect(int index) { return insects.at(index); }

    int GetinsectOneIndex() const { return insectOneIndex; }

    void Remove(Insect* enemy);

private:
    std::vector<Insect*> insects;
    std::vector<Insect*> removes;

    const int insectOneIndex = 0;
};