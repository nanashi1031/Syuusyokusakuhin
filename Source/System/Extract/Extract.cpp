#include "Extract.h"

void Extract::Initialize()
{
    extractUI1 = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUI1.png");
    extractUI2 = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUI2.png");
    extractUI3 = std::make_unique<Sprite>("Data/Sprite/UI/ExtractUI3.png");

    for (int i = 0; i < extractMax; i++)
    {
        extract.emplace_back(-0.01f);
    }
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
}

void Extract::Render2D(ID3D11DeviceContext* dc)
{
    float textureWidth = static_cast<float>(extractUI1->GetTextureWidth());
    float textureHeight = static_cast<float>(extractUI1->GetTextureHeight());

    DirectX::XMFLOAT4 color;
    if (extract[0] > 0.00f)
        color = Extract::Instance().ColorConversion(ExtractColor::Red);
    else
        color = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[0] < 5.0f)
    {
        if (colorFlag)
        {
            color.x += 0.02f;
            if (color.x > 1.0f)
                colorFlag = false;
        }
        else if (!colorFlag)
        {
            color.x -= 0.02f;
            if (color.x < 0.0f)
                colorFlag = true;
        }
    }

    extractUI1->Render(
        dc,
        100, 0,
        textureWidth / 2, textureHeight / 2,
        0, 0,
        textureWidth, textureHeight,
        0,
        color.x, color.y, color.z, color.w
    );

    textureWidth = static_cast<float>(extractUI2->GetTextureWidth());
    textureHeight = static_cast<float>(extractUI3->GetTextureHeight());

    if (extract[1] > 0.00f)
        color = Extract::Instance().ColorConversion(ExtractColor::White);
    else
        color = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[1] < 5.0f)
    {
        if (colorFlag)
        {
            color.x += 0.02f;
            color.y += 0.02f;
            color.z += 0.02f;
            if (color.x > 1.0f)
                colorFlag = false;
        }
        else if (!colorFlag)
        {
            color.x -= 0.02f;
            color.y -= 0.02f;
            color.z -= 0.02f;
            if (color.x <= 0.0f)
                colorFlag = true;
        }
    }

    extractUI2->Render(
        dc,
        170, 0,
        textureWidth / 2, textureHeight / 2,
        0, 0,
        textureWidth, textureHeight,
        0,
        color.x, color.y, color.z, color.w
    );

    textureWidth = static_cast<float>(extractUI3->GetTextureWidth());
    textureHeight = static_cast<float>(extractUI3->GetTextureHeight());

    if (extract[2] > 0.00f)
        color = Extract::Instance().ColorConversion(ExtractColor::Orange);
    else
        color = Extract::Instance().ColorConversion(ExtractColor::None);

    if (extract[2] < 5.0f)
    {
        if (colorFlag)
        {
            color.x += 0.02f;
            color.y += 0.01f;
            if (color.x > 1.0f)
                colorFlag = false;
        }
        else if (!colorFlag)
        {
            color.x -= 0.02f;
            color.y -= 0.01f;
            if (color.x <= 0.0f)
                colorFlag = true;
        }
    }

    if (extract[2] < 5.0f)
    {
        if (colorFlag)
        {
            color.x += 0.02f;
            color.y += 0.02f;
            color.z += 0.02f;
            if (color.x > 1.0f)
                colorFlag = false;
        }
        else if (!colorFlag)
        {
            color.x -= 0.02f;
            color.y -= 0.02f;
            color.z -= 0.02f;
            if (color.x <= 0.0f)
                colorFlag = true;
        }
    }

    extractUI3->Render(
        dc,
        240, 0,
        textureWidth / 2, textureHeight / 2,
        0, 0,
        textureWidth, textureHeight,
        0,
        color.x, color.y, color.z, color.w
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