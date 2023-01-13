#include "PlayerManager.h"

void PlayerManager::Clear()
{
    for (Player* player : players)
    {
        delete player;
    }
}

void PlayerManager::Update(float elapsedTime)
{
    for (Player* player : players)
    {
        player->Update(elapsedTime);
    }

    // ”jŠüˆ—
    for (Player* player : removes)
    {
        std::vector<Player*>::iterator it = std::find(
            players.begin(), players.end(), player);

        if (it != players.end())
        {
            players.erase(it);
        }

        delete player;
    }
    removes.clear();
}

void PlayerManager::Render(RenderContext rc, ModelShader* shader)
{
    for (Player* player : players)
    {
        player->Render(rc, shader);
    }
}

void PlayerManager::Register(Player* player)
{
    players.emplace_back(player);
}

void PlayerManager::DrawDebugPrimitive()
{
    for (Player* player : players)
    {
        player->DrawDebugPrimitive();
    }
}

void PlayerManager::DrawDebugGUI()
{
    for (Player* player : players)
    {
        player->DrawDebugGUI();
    }
}


void PlayerManager::Remove(Player* player)
{
    //”jŠüƒŠƒXƒg‚É’Ç‰Á
    removes.emplace_back(player);
}