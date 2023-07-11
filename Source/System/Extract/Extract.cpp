#include "Extract.h"

void Extract::Initialize()
{
    extractUIRed = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUISword.png");
    extractUIWhite = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUIShoes.png");
    extractUIOrange = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUIShield.png");

    for (int i = 0; i < extractMax; i++)
    {
        extract.emplace_back();
    }
}

void Extract::Finalize()
{
}

void Extract::Update(float elapsedTime)
{
    for (int i = 0; i < extractMax; i++)
    {
        if (extract[i] > 0.0f)
        {
            extract[i] -= 1.0f * elapsedTime;
        }
    }

    ExtractUISlowFlashing(0.01f);
    ExtractUIFastFlashing(0.03f);
}


void Extract::Render2D(ID3D11DeviceContext* dc)
{
    RenderRedUI(dc);
    RenderWhiteUI(dc);
    RenderOrangeUI(dc);
}

void Extract::RenderRedUI(ID3D11DeviceContext* dc)
{
    Graphics& graphics = Graphics::Instance();

    DirectX::XMFLOAT4 redUIColor;
    if (extract[static_cast<int>(ExtractColor::Red)] > 0.00f)
        redUIColor = Extract::Instance().ColorConversion(ExtractColor::Red);
    else
        redUIColor = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[static_cast<int>(ExtractColor::Red)] > limitTime)
    {
        redUIColor.x = colorAccumulationSlow;
    }
    else if (extract[static_cast<int>(ExtractColor::Red)] > 0.0f)
    {
        redUIColor.x = colorAccumulationFast;
    }

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenWidth());
    float textureWidth = static_cast<float>(extractUIRed->GetTextureWidth());
    float textureHeight = static_cast<float>(extractUIRed->GetTextureHeight());
    extractUIRed->Render(
        dc,
        screenWidth / 9.0f, screenWidth / 15.0f,
        textureWidth / 1.5f, textureHeight / 1.5f,
        0, 0,
        textureWidth, textureHeight,
        0,
        redUIColor.x, redUIColor.y, redUIColor.z, redUIColor.w
    );
}

void Extract::RenderWhiteUI(ID3D11DeviceContext* dc)
{
    Graphics& graphics = Graphics::Instance();

    DirectX::XMFLOAT4 whiteUIColor;
    if (extract[static_cast<int>(ExtractColor::White)] > 0.00f)
        whiteUIColor = Extract::Instance().ColorConversion(ExtractColor::White);
    else
        whiteUIColor = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[static_cast<int>(ExtractColor::White)] > limitTime)
    {
        whiteUIColor.x = colorAccumulationSlow;
        whiteUIColor.y = colorAccumulationSlow;
        whiteUIColor.z = colorAccumulationSlow;
    }
    else if (extract[static_cast<int>(ExtractColor::White)] > 0.0f)
    {
        whiteUIColor.x = colorAccumulationFast;
        whiteUIColor.y = colorAccumulationFast;
        whiteUIColor.z = colorAccumulationFast;
    }

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenWidth());
    float textureWidth = static_cast<float>(extractUIWhite->GetTextureWidth());
    float textureHeight = static_cast<float>(extractUIWhite->GetTextureHeight());
    extractUIWhite->Render(
        dc,
        screenWidth / 6.5f, screenWidth / 17.0f,
        textureWidth / 1.1f, textureHeight / 0.8f,
        0, 0,
        textureWidth, textureHeight,
        0,
        whiteUIColor.x, whiteUIColor.y, whiteUIColor.z, whiteUIColor.w
    );
}

void Extract::RenderOrangeUI(ID3D11DeviceContext* dc)
{
    Graphics& graphics = Graphics::Instance();

    DirectX::XMFLOAT4 orangeUIColor;
    if (extract[static_cast<int>(ExtractColor::Orange)] > 0.00f)
        orangeUIColor = Extract::Instance().ColorConversion(ExtractColor::Orange);
    else
        orangeUIColor = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[static_cast<int>(ExtractColor::Orange)] > limitTime)
    {
        orangeUIColor.x = colorAccumulationSlow;
        orangeUIColor.y = colorAccumulationSlow / 2;
    }
    else if (extract[static_cast<int>(ExtractColor::Orange)] > 0.0f)
    {
        orangeUIColor.x = colorAccumulationFast;
        orangeUIColor.y = colorAccumulationFast / 2;
    }

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenWidth());
    float textureWidth = static_cast<float>(extractUIOrange->GetTextureWidth());
    float textureHeight = static_cast<float>(extractUIOrange->GetTextureHeight());
    extractUIOrange->Render(
        dc,
        screenWidth / 4.4f, screenWidth / 15.0f,
        textureWidth / 1.8f, textureHeight / 1.5f,
        0, 0,
        textureWidth, textureHeight,
        0,
        orangeUIColor.x, orangeUIColor.y, orangeUIColor.z, orangeUIColor.w
    );
}

void Extract::DrawDebugGUI()
{
    ImVec2 windowPosition = { 10, 10 };
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
    ImVec2 windowSize = { 300, 300 };
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);

    if (ImGui::Begin("Extract", nullptr))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::DragFloat("positiona", &a, 0.1f);
            ImGui::DragFloat("positionb", &b, 0.1f);
            ImGui::DragFloat("positionc", &c, 0.1f);
            ImGui::DragFloat("positiond", &d, 0.1f);

            ImGui::SliderFloat("timerRed", &extract[0], 0.0f, extractTime);
            ImGui::SliderFloat("timerWhite", &extract[1], 0.0f, extractTime);
            ImGui::SliderFloat("timerOrange", &extract[2], 0.0f, extractTime);
        }
    }
    ImGui::End();
}

void Extract::ExtractUISlowFlashing(float colorPower)
{
    if (colorFlagSlow)
    {
        colorAccumulationSlow += colorPower;
        if (colorAccumulationSlow > 1.0f)
        {
            colorAccumulationSlow = 1.0f;
            colorFlagSlow = false;
        }
    }
    else if (!colorFlagSlow)
    {
        colorAccumulationSlow -= colorPower;
        if (colorAccumulationSlow < 0.5f)
        {
            colorAccumulationSlow = 0.5f;
            colorFlagSlow = true;
        }
    }
}

void Extract::ExtractUIFastFlashing(float colorPower)
{
    if (colorFlagFast)
    {
        colorAccumulationFast += colorPower;
        if (colorAccumulationFast > 0.7f)
        {
            colorAccumulationFast = 0.7f;
            colorFlagFast = false;
        }
    }
    else if (!colorFlagFast)
    {
        colorAccumulationFast -= colorPower;
        if (colorAccumulationFast < 0.0f)
        {
            colorAccumulationFast = 0.0f;
            colorFlagFast = true;
        }
    }
}