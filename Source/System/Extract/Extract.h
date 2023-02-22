#pragma once
#include <DirectXMath.h>
#include"Graphics.h"

enum class ExtractColor
{
    Red,
    White,
    Orange,
    Heal,
    None,
};

class Extract
{
private:
    struct ExtractInfo
    {
        int extractColor;
        float timer;
    };

public:
    static Extract& Instance()
    {
        static Extract extract;
        return extract;
    }

    void Initialize();
    void Finalize();
    void Update(float elapsedTime);
    void Render2D(ID3D11DeviceContext* dc);
    void DrawDebugGUI();

    template<typename T>
    DirectX::XMFLOAT4 ColorConversion(T colorNo)
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
            outColor = { 0.0f, 0.0f, 0.0f, 1.0f };

        return outColor;
    }

    template <typename T>
    float GetExtract(T index) { return extract[static_cast<int>(index)]; }

    template<typename T>
    void SetExtract(T color)
    {
        switch (static_cast<ExtractColor>(color))
        {
        case ExtractColor::Red:
            extract[0] = extractTime;
            break;
        case ExtractColor::White:
            extract[1] = extractTime;
            break;
        case ExtractColor::Orange:
            extract[2] = extractTime;
            break;
        case ExtractColor::Heal:
            break;
        case ExtractColor::None:
            break;
        }
    }

private:
    std::vector<float> extract;

    const float extractTime = 30.0f;
    const int extractMax = 3;
    float colors = 0.0f;
    float colorFlag = false;
    float a, b, c, d = 1;

    std::unique_ptr<Sprite> extractUI1 = nullptr;
    std::unique_ptr<Sprite> extractUI2 = nullptr;
    std::unique_ptr<Sprite> extractUI3 = nullptr;
};