#pragma once
#include <DirectXMath.h>

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

};