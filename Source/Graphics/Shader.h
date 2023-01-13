#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "RenderContext.h"
#include "Model.h"
#include "Sprite.h"

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	// �`��J�n
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model) = 0;

	// �`��I��
	virtual void End(ID3D11DeviceContext* context) = 0;
};

class ModelShader
{
public:
	ModelShader() {}
	virtual ~ModelShader() {}

	// �`��J�n
	virtual void Begin(const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(const RenderContext& rc, const Model* model) = 0;

	// �`��I��
	virtual void End(const RenderContext& rc) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// �`��J�n
	virtual void Begin(const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(const RenderContext& rc, const Sprite* sprite) = 0;

	// �`��I��
	virtual void End(const RenderContext& rc) = 0;
};