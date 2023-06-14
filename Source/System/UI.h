#pragma once
#include <DirectXMath.h>
#include"Graphics.h"
#include <vector>

class UI
{
public:
    struct UIGauge
    {
        const char* name;
        float frame;
        float gage;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 width;
        DirectX::XMFLOAT2 height;
        float angle;
        DirectX::XMFLOAT4 color;
    };

    struct DamegeUI
    {
        float damege;
        float timer;
        DirectX::XMFLOAT2 position;
    };

    void Initialize();
    void Finalize();
    void Update(float elapsedTime);
    void Render2D(ID3D11DeviceContext* dc);
    void DrawDebugGUI();

    std::vector<float> parts;
};