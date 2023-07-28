#include "UI.h"
#include "Mathf.h"

void UI::Initialize()
{
    spr_damegeUI = std::make_unique<Sprite>("Data/Sprite/UI/Suuzi.png");
}

void UI::Finalize()
{

}

void UI::Update(float elapsedTime)
{
    UpdateDamegeUI(elapsedTime);
}

void UI::Render2D(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    Render2DDamegeUI(dc, view, projection);
}

void UI::UpdateDamegeUI(float elapsedTime)
{
    int index = static_cast<int>(damegeUIs.size());
    for (int i = 0; i < index; i++)
    {
        damegeUIs[i].position.y += elapsedTime / 5.0f;
        damegeUIs[i].timer += elapsedTime;

        if (damegeUIs[i].timer > maxTimer)
        {
            damegeUIs.erase(damegeUIs.begin() + i);
            index--;
        }
    }
}

void UI::Render2DDamegeUI(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    Graphics& graphics = Graphics::Instance();
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(spr_damegeUI->GetTextureWidth());
    float textureHeight = static_cast<float>(spr_damegeUI->GetTextureHeight());

    int index = static_cast<int>(damegeUIs.size());
    for (int i = 0; i < index; i++)
    {
        DirectX::XMFLOAT2 screenPosition =
            Mathf::ConvertWorldToScreen(damegeUIs[i].position, dc, view, projection);

        float alpha = 1.0f;
        float transparentTimer = 1.4f;
        if(damegeUIs[i].timer > transparentTimer)
            alpha = 1.0f - Mathf::Percentage(damegeUIs[i].timer, maxTimer);


        int digit = Mathf::CountDigits(damegeUIs[i].damege);
        int damege = static_cast<int>(damegeUIs[i].damege);
        for (int j = 0; j < digit; j++)
        {
            // 最小桁の数を取得
            int num = damege % 10;
            // 最小桁を消す
            damege /= 10;

            // 0~9まで数字が10個あるため10で割れば数1個のサイズがわかる
            float numSize = textureWidth / 10.0f;
            // 数字の実際に表示するサイズ(横幅)
            float kazuWidth = textureWidth / 30.0f;
            // 数字の実際に表示するサイズ(縦幅)
            float kazuHeight = textureHeight / 3.0f;

            spr_damegeUI->Render(
                dc,
                screenPosition.x - (kazuWidth * j), screenPosition.y,
                kazuWidth, kazuHeight,
                numSize * num, 0,
                numSize, textureHeight,
                0,
                1.0f, 1.0f, 1.0f, alpha
            );
        }
    }
}

void UI::DrawDebugGUI()
{
    ImVec2 windowPosition = { 10, 10 };
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);
    ImVec2 windowSize = { 300, 300 };
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    // ウィンドウの透明度
    float alpha = 0.35f;
    ImGui::SetNextWindowBgAlpha(alpha);

    if (ImGui::Begin("UI", nullptr))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Information", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int index = static_cast<int>(damegeUIs.size());
            if (index > 0)
            {
                float damage = damegeUIs[index - 1].damege;
                ImGui::DragFloat("damage", &damage, 0.1f);
            }
        }
    }
    ImGui::End();
}