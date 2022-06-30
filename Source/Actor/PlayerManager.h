#pragma once

#include <vector>
#include "Player.h"

//プレイヤーマネージャー
class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Clear();

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void Register(Player* enemy);

    void DrawDebugPrimitive();

    void DrawDebugGUI();

    int GetPlayerCount() const { return static_cast<int>(players.size()); }

    Player* GetPlayer(int index) { return players.at(index); }

    int GetplayerOneIndex() const { return playerOneIndex; }

    void Remove(Player* enemy);

private:
    std::vector<Player*> players;
    std::vector<Player*> removes;

    const int playerOneIndex = 0;
};