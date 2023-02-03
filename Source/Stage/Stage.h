#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Object.h"
#include "Collision.h"

// �X�e�[�W
class Stage : public Object
{
public:
    Stage() {};
    virtual ~Stage() {};

    virtual void Update(float elapsedTime) = 0;

    virtual void Render(RenderContext rc, ModelShader* shader) = 0;

    virtual void DrawDebugGUI() = 0;

    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
    {
        return Collision::IntersectRayVsModel(start, end, model, hit);
    }

    // �Q�b�^�[�@�Z�b�^�[
    Model* GetModel() const { return model; }
};