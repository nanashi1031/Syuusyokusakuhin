#include "Insect.h"
#include "PlayerManager.h"

Insect::Insect()
{
    model.reset(new Model("Data/Model/SpikeBall/SpikeBall.mdl"));
    scale.x = scale.y = scale.z = size;

    radius = 0.5f;
    height = 2.0f;
}

Insect::~Insect()
{

}

void Insect::Update(float elapsedTime)
{
    UpdateTransform();

    model->UpdateTransform(transform);

    Insect::PlayerWeaponTracking(elapsedTime);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void Insect::DrawDebugPrimitive()
{

}

void Insect::DrawDebugGUI()
{

}

void Insect::PlayerWeaponTracking(float elapsedTime)
{
    PlayerManager& playerManager = PlayerManager::Instance();

    Model::Node* playerWeponFront = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandIndex1");
    Model::Node* playerWeponBack = playerManager.GetPlayer(playerManager.GetplayerOneIndex())->GetNode("mixamorig:RightHandMiddle1");

    DirectX::XMFLOAT3 playerWeponFrontPosition;
    playerWeponFrontPosition.x = playerWeponFront->worldTransform._41;
    playerWeponFrontPosition.y = playerWeponFront->worldTransform._42;
    playerWeponFrontPosition.z = playerWeponFront->worldTransform._43;

    position = playerWeponFrontPosition;
}