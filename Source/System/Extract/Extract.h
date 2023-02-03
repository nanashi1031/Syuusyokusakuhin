#pragma once
#include <DirectXMath.h>

enum class ExtractColor
{
    None,
    Red,
    White,
    Orange,
    Heal,
};

class Extract
{
public:
    template<typename T>
    static DirectX::XMFLOAT4 ColorConversion(T colorNo)
    {
        ExtractColor color = static_cast<ExtractColor>(colorNo);
        DirectX::XMFLOAT4 outColor;
        if (color == ExtractColor::Red)
            outColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        else if (color == ExtractColor::White)
            outColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        else if (color == ExtractColor::Orange)
            outColor = { 1.0f, 0.5f, 0.0f, 1.0f };
        else if (color == ExtractColor::Heal)
            outColor = { 0.0f, 1.0f, 0.0f, 1.0f };
        else
            outColor = { 1.0f, 1.0f, 1.0f, 1.0f };

        return outColor;
    }
};