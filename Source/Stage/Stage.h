#pragma once

#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Object.h"

// ステージ
class Stage : public Object
{
public:
    Stage() {};
    virtual ~Stage() {};

    virtual void Update(float elapsedTime) = 0;

    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
};