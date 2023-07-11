#include "InsectManager.h"

void InsectManager::Initialize()
{
    for (Insect* insect : insects)
    {
        insect->Initialize();
    }
}

void InsectManager::Clear()
{
    insects.clear();
}

void InsectManager::Update(float elapsedTime)
{
    for (Insect* insect : insects)
    {
        insect->Update(elapsedTime);
    }

    // ”jŠüˆ—
    for (Insect* insect : removes)
    {
        std::vector<Insect*>::iterator it = std::find(
            insects.begin(), insects.end(), insect);

        if (it != insects.end())
        {
            insects.erase(it);
        }

        delete insect;
    }
    removes.clear();
}

void InsectManager::Render(RenderContext rc, ModelShader* shader)
{
    for (Insect* insect : insects)
    {
        insect->Render(rc, shader);
    }
}

void InsectManager::Register(Insect* insect)
{
    insects.emplace_back(insect);
}

void InsectManager::DrawDebugPrimitive()
{
    for (Insect* insect : insects)
    {
        insect->DrawDebugPrimitive();
    }
}

void InsectManager::DrawDebugGUI()
{
    for (Insect* insect : insects)
    {
        insect->DrawDebugGUI();
    }
}


void InsectManager::Remove(Insect* insect)
{
    //”jŠüƒŠƒXƒg‚É’Ç‰Á
    removes.emplace_back(insect);
}