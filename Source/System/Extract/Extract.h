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
    void RenderButterfly(ID3D11DeviceContext* dc);
    void RenderRedUI(ID3D11DeviceContext* dc);
    void RenderWhiteUI(ID3D11DeviceContext* dc);
    void RenderOrangeUI(ID3D11DeviceContext* dc);
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

    // エキスを所持しているかどうか判定
    bool HaveExtract()
    {
        for (int i = 0; i < extractMax; i++)
        {
            if (extract[i] > 0.0f)
                return true;
        }
        return false;
    }

private:
    // エキスの点滅処理
    void ExtractUISlowFlashing(float colorPower);
    // エキスが切れそうになったら点滅処理
    void ExtractUIFastFlashing(float colorPower);

private:
    std::vector<float> extract;

    const float extractTime = 30.0f;
    const int extractMax = 3;
    bool colorFlagSlow = false;
    bool colorFlagFast = false;
    float colorAccumulationSlow = 0.0f;
    float colorAccumulationFast = 0.0f;
    const float maxTime = 30.0f;
    const float limitTime = 10.0f;
    float a, b, c, d = 1;

    std::unique_ptr<Sprite> spr_butterfly = nullptr;

    std::unique_ptr<Sprite> extractUIRed = nullptr;
    std::unique_ptr<Sprite> extractUIWhite = nullptr;
    std::unique_ptr<Sprite> extractUIOrange = nullptr;
};