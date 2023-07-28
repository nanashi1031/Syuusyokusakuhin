#pragma once
#include <DirectXMath.h>
#include"Graphics.h"
#include <vector>

class UI
{
public:
    static UI& Instance()
    {
        static UI ui;
        return ui;
    }

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
        DirectX::XMFLOAT3 position;
    };

    void Initialize();
    void Finalize();
    void Update(float elapsedTime);
    void Render2D(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);
    void DrawDebugGUI();

private:
    void UpdateDamegeUI(float elapsedTime);
    void Render2DDamegeUI(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);

public:
    // セッター　ゲッター
    void SetDamegeUI(float damege, DirectX::XMFLOAT3 position, float timer = 0.0f)
    {
        DamegeUI damegeUI;
        damegeUI.damege = damege;
        damegeUI.timer = timer;
        damegeUI.position = position;
        damegeUIs.emplace_back(damegeUI);
    };
    std::vector<DamegeUI> GetDamegeUI() const { return this->damegeUIs; }

private:
    std::vector<float> parts;
    std::vector<DamegeUI> damegeUIs;

    std::unique_ptr<Sprite> spr_damegeUI = nullptr;

    const float maxTimer = 2.4f;
};